#include <iostream>
#include <fstream>
#include <cstring>
//save_yura_give_60

//константы для меню
#define ADD_DISK 1//добавление элемента
#define PRINT_LIST 2//вывод структуры
#define CHANGE_DISK 3//смена значений определенного элемента
#define AMOUNT 4//подсчет кол-ва элементов
#define DELETE 5//удаление элемента
#define YEAR_AND_HIGHER 6//сортировка элементов по году (от n-го и выше)
#define YEAR 7//сортировка элементов по году (по возрастаию)
#define DURATION 8//сортировка элементов по длительности (от n-ой и больше)
#define ALBUM 9//вывод самого раннего альбома исполнителя
#define QUIT 0//выход

using namespace std;

//упрощение инициализации структуры
typedef struct disk disk;

struct disk {
    char *name;
    int year;
    int amount;
    int duration;
    char *author;
    int id;
    disk *next;
};

static disk *front = NULL;

//прототипы функций
int inputMenuOption();//вывод меню
void pauseConsole();//заморозка консоли
void inputDiskFromConsole();//ввод диска пользователем
void addDiskToList(disk *ptr);//добавление элементов в структуру
void printList();//вывод структуры
void changeDisk();//смена значений определенного элемента
void readDisksFromFile();//чтение из файла
int amountOfDisks(disk *front);//подсчет кол-ва элементов
int inputDeletePos();//ввод позицию элемента для удаления
void deleteDisk(disk **front);//удаление элемента
void sortByYearAndHigher();//сортировка элементов по году (от n-го и выше)
void sortedByYear();//сортировка элементов по году (по возрастаию)
void sortByDuration();//сортировка элементов по длительности (от n-ой и больше)
void earliestAlbum();//вывод самого раннего альбома исполнителя
void writeDisksToFile();//запись в файл
int isInt(); //проверка на ввод числа
char *isSpace(); //проверка на наличие пробелов в строке
int main() {
    //читаем файл и заполняем структуру внутренними данными
    readDisksFromFile();
    //получение выбора пользователя
    int option = inputMenuOption();

    while (option != QUIT) {
        switch (option) {
            case ADD_DISK://1
                inputDiskFromConsole();
                break;
            case PRINT_LIST://2
                printList();
                break;
            case CHANGE_DISK://3
                changeDisk();
                break;
            case AMOUNT://4
                cout << "Amount of products: " << amountOfDisks(front);
                break;
            case DELETE://5
                deleteDisk(&front);
                break;
            case YEAR_AND_HIGHER://6
                sortByYearAndHigher();
                break;
            case YEAR://7
                sortedByYear();
                break;
            case DURATION://8
                sortByDuration();
                break;
            case ALBUM://9
                earliestAlbum();
                break;
            default://если вдруг пользователь не умеет читать
                printf("\n\nWrong choice!");
                break;
        }
        //возврашение к выбору
        option = inputMenuOption();
    }

    cout << "\nBye!";
    writeDisksToFile();//сохраняем все данные перед выходом в файл
    pauseConsole();

    return 0;
}

int inputMenuOption() {
    //перечень функций программы
    cout << "\n----DISKS----\n\n";
    cout << "Menu:\n";
    cout << "1.\tAdd new disk\n";
    cout << "2.\tPrint all disks\n";
    cout << "3.\tChange certain disk\n";
    cout << "4.\tShow amount of disks\n";
    cout << "5.\tDelete certain disk\n";
    cout << "6.\tShow disks by certain year and higher\n";
    cout << "7.\tSort by year\n";
    cout << "8.\tSort by certain duration and higher\n";
    cout << "9.\tShow the earliest album of certain artist\n";
    cout << "0.\tQuit\n\n";
    cout << "Make your selection (ONLY DIGITS): ";

    int option;
    option = isInt();
    return option;
}

int isInt(){
    char test_s[30];
    int test;

    while(1){
        cin >> test_s;
        //функция atoi возвращает 0, если пользователь не ввел число
        if ((atoi(test_s) == 0 && (test_s[0] != '0'))){
            cout << "Write number: ";
        }
        else {
            //если пользователь все таки ввел число
            test = atoi(test_s);
            //если оно меньше 0, то
            if (test < 0){
                //заставляем ввести нормальное число
                do{
                    cout << "Write right number: ";
                    cin >> test;
                } while (test < 0);
            }
            return test;
        }
    }
}

char *isSpace(){
    string check;
    int i;
    //игнорируем первый ввод
    cin.ignore();
    //получаем всю введенную строку
    getline(cin, check);
    //прочесываем строку в поиске пробелов
    for(i = 0; i < check.length(); i++){
        if(isspace(check[i])){
            //если нашли пробел, меняем на нижнее подчеркивание
            check[i] = '_';
        }
    }
    //выделяем память под чар, копируем полученный результат
    char* buf = new char[30];
    strcpy(buf,check.c_str());
    //возвращаем нуждающемуся
    return buf;
}

void pauseConsole() {
    //замораживаем программу, пока пользователь не нажмет Enter
    cout << "\n\nPress any key to continue...";
    while (getchar() != '\n');
    getchar();
}

void readDisksFromFile(){
    //ищем и открываем файл из которого читаем
    ifstream musicFileRead("data.txt");
    ifstream text("data.txt");
    string s;
    int i, n = 0;
    //считаем кол-во строк в файле
    while(getline(text, s)){
        n++;
    }
    //т.к. в каждом "диске" 5 элементов для ввода,
    //делим кол-во строк на 5 и равномерно все вписываем
    for(i = 0; i < n/5; i++){
        //выделение памяти
        disk *ptr = static_cast<disk *>(malloc(sizeof(disk)));
        //создаем пустой элемент
        ptr->next = NULL;
        //выделение памяти
        ptr->name = static_cast<char *>(malloc(64 * sizeof(char)));
        ptr->author = static_cast<char *>(malloc(64 * sizeof(char)));
        //читаем из файла
        musicFileRead >> ptr->name >> ptr->year >> ptr->amount >> ptr->duration >> ptr->author;
        //передаем записанный элемент в функцию для добавления в структуру
        addDiskToList(ptr);
    }
    //закрываем файл
    musicFileRead.close();

}

void writeDisksToFile(){
    //открываем файл для записи в него данных
    ofstream musicFileWrite("data.txt");
    //создаем указатель для перебора всей структуры
    disk *tmp = front;

    //перебираем структуру и вписываем все данные в файл
    while(tmp != NULL){
        musicFileWrite << "\n" << tmp->name;
        musicFileWrite << "\n" << tmp->year;
        musicFileWrite << "\n" << tmp->amount;
        musicFileWrite << "\n" << tmp->duration;
        musicFileWrite << "\n" << tmp->author;
        //переключаемся на следующий элемент
        tmp = tmp->next;
    }
}

void inputDiskFromConsole() {

    cout << "\nADDING NEW DISK:\n";
    //создаем указатель для перебора всей структуры
    disk *tmp = front;
    //выделение памяти
    disk *ptr = static_cast<disk *>(malloc(sizeof(disk)));
    //создаем пустой элемент
    ptr->next = NULL;

    //выделение памяти
    ptr->name = static_cast<char *>(malloc(64 * sizeof(char)));

    cout << "\nName: ";
    //вписываем название диска
    ptr->name = isSpace();

    //проверяем, нет ли уже диска с таким названием
    while (tmp != NULL) {
        if(strcmp(ptr->name, tmp->name) == 0){
            //если есть, то
            do{
                //говорим что такой уже есть и даем попробовать снова
                //пока не будет введено уникальное имя
                cout << "\n" << ptr->name << " already exists. Try again.\n";
                ptr->name = isSpace();
            }while(strcmp(ptr->name, tmp->name) == 0);
            //переключаемся в начало структуры
            tmp = front;
            //проверяем, нет ли уже диска с таким названием
            if(strcmp(ptr->name, tmp->name) == 0){
                //если есть, то
                do{
                    //говорим что такой уже есть и даем попробовать снова
                    //пока не будет введено уникальное имя
                    cout << "\n" << ptr->name << " already exists. Try again.\n";
                    ptr->name = isSpace();
                }while(strcmp(ptr->name, tmp->name) == 0);
            }
        }
        //следующий элемент
        tmp = tmp->next;
    }

    //последующий ввод остальных данных
    cout << "\nYear:";
    ptr->year = isInt();

    cout <<"\nAmount of songs:";
    ptr->amount = isInt();

    cout << "\nDuration (in min):";
    ptr->duration = isInt();

    //выделение памяти
    ptr->author = static_cast<char *>(malloc(64 * sizeof(char)));
    cout << "\nAuthor: ";
    ptr->author = isSpace();


    addDiskToList(ptr);
}

void addDiskToList(disk *ptr) {
    //если список пустой
    if (front == NULL) {
        front = ptr;
        //приписываем элементу первый номер
        front->id = amountOfDisks(front);
        return;
    }

    disk *tmp = front;
    //ищем последний элемент структуры
    while (tmp->next != NULL) {
        tmp = tmp->next;
    }
    //приписываем соответствующий номер
    ptr->id = amountOfDisks(front)+1;
    //добавляем в структуру
    tmp->next = ptr;

}

void printList() {
    cout << "\nLIST OF DISKS: \n\n";

    if (front == NULL) {
        cout << "LIST IS EMPTY! \n\n";
        return;
    }

    disk *tmp = front;

    //выводим всю структуру
    while (tmp != NULL) {
        cout << "-----------------\n";
        cout << "NAME: " << tmp->name;
        cout << "\nYEAR: " << tmp->year;
        cout << "\nAMOUNT OF SONGS: " << tmp->amount;
        cout << "\nDURATION: " << tmp->duration;
        cout << "\nAUTHOR: " << tmp->author;
        cout << "\nID: " << tmp->id;
        cout << "\n-----------------\n";

        tmp = tmp->next;
    }

    pauseConsole();
}

void changeDisk(){
    //выделение памяти
    char *diskName = new char[80];

    if (front == NULL) {
        cout << "LIST IS EMPTY! \n\n";
        return;
    }

    disk *tmp = front;

    int i = 0;
    cout << "SEARCH DISK BY NAME\n";
    cout << "\nEnter disk name:";
    diskName = isSpace();

    while (tmp != NULL) {
        //ищем элемент с таким же названием
        if(strcmp(diskName, tmp->name) == 0) {
            cout << "-----------------\n";
            cout << "Enter new name for disk (use underscale(_) instead of space): ";
            tmp->name = isSpace();
            cout << "Year: ";
            tmp->year = isInt();
            cout << "Amount of songs:";
            tmp->amount = isInt();
            cout << "Duration (in min): ";
            tmp->duration = isInt();
            cout << "Author (use underscale(_) instead of space): ";
            tmp->author = isSpace();
            cout << "\n-----------------\n";
            i++;//флажок
        }
        tmp = tmp->next;
    }
    //если в структуре нет диска с таким названием, то
    if(!i){
        //предлагаем попробовать еще раз
        int choice;
        while(1){
            cout << "\nWrong input\n";
            cout << "Do you want to try again?(1-yes, 2-no)";
            choice = isInt();
            if(choice == 1 || choice == 2){
                break;
            }
        }
        if(choice == 1){
            //вызываем функцию заново
            void (*func) ();
            func = changeDisk;
            func();
            return;
        }
        //else if(choice == 2){

        //}
    }
    pauseConsole();
}

int amountOfDisks(disk *front){
    int count = 0;
    if(front == NULL){
        cout << "\nList is empty\n";
    }
    //тот же самый перебор структуры, для подсчета кол-ва элементов
    disk *ptr = NULL;
    ptr = front;
    while(ptr != NULL){
        count++;
        ptr = ptr->next;
    }

    return count;
}

int inputDeletePos(){
    int count = 0;
    count = amountOfDisks(front);
    int choice;

    cout << "Amount of disks: " << count;
    cout << "\nEnter position of disk:\n ";
    choice = isInt();

    //проверяем, не выходит ли выбор за пределы структуры
    if(choice > amountOfDisks(front) || choice < 1){
        //если да, предлагаем попробовать еще раз
        cout << "Wrong position!\n";
        cout << "Do you want to try again?(1-yes, 2-no)";
        choice = isInt();
        if(choice == 1){
            //вызываем функцию заново
            int (*func) ();
            func = inputDeletePos;
            func();
        }
    }
    int position = choice;
    return position;
}

void deleteDisk(disk **front){
    int position;
    //получаем позицию неугодного нам диска
    position = inputDeletePos();

    //добавляем указатели
    disk *current = *front;
    disk *previous = *front;

    if(*front == NULL){
        printf("\nList is empty\n");
    }
        //повезло, элемент первый по счету, можно не перебирать
    else if (position == 1){
        //очищаем память и удаляем элемент
        *front = current->next;
        free(current);
        current = NULL;
    }
    else{
        //не повезло, надо перебирать структуру
        while (position != 1){
            previous = current;
            current = current->next;
            position--;
        }
        //нашли нужный элемент
        //очищаем память и удаляем элемент
        previous->next = current->next;
        free(current);
        current = NULL;
    }
    cout << "\nDisk has been deleted\n";
    pauseConsole();
}

void sortByYearAndHigher(){

    disk *tmp = front;
    int chosenYear;

    cout << "Which year are you interested in?\n";
    chosenYear = isInt();

    while (tmp != NULL) {
        //перебираем структуру, если год больше или равен введенному, то
        if(chosenYear <= tmp->year){
            //выводим подходящий элемент
            cout << "-----------------\n";
            cout << "NAME: " << tmp->name;
            cout << "\nYEAR: " << tmp->year;
            cout << "\nAMOUNT OF SONGS: " << tmp->amount;
            cout << "\nDURATION: " << tmp->duration;
            cout << "\nAUTHOR: " << tmp->author;
            cout << "\nID: " << tmp->id;
            cout << "\n-----------------\n";
        }
        tmp = tmp->next;
    }
    pauseConsole();
}

void sortedByYear(){
    disk *tmp = front;

    int i, j, n = 0, damn;
    int allYears[amountOfDisks(front)];

    //собираем года всех дисков и считаем сколько их
    while (tmp != NULL) {
        allYears[n] = tmp->year;
        n++;
        tmp = tmp->next;
    }
    //возвращаемся на первый элемент структуры
    tmp = front;

    //сортируем года по возрастанию пузырьком
    for(i = 0; i < n; i++){
        for(j = 0; j < n-1; j++){
            if(allYears[j] > allYears[j+1]){
                damn = allYears[j];
                allYears[j] = allYears[j+1];
                allYears[j+1] = damn;
            }
        }
    }

    //выводим все элементы в нужном порядке
    for(i = 0; i < n; i++){
        while (tmp != NULL) {
            if(allYears[i] == tmp->year){
                cout << "-----------------\n";
                cout << "NAME: " << tmp->name;
                cout << "\nYEAR: " << tmp->year;
                cout << "\nAMOUNT OF SONGS: " << tmp->amount;
                cout << "\nDURATION: " << tmp->duration;
                cout << "\nAUTHOR: " << tmp->author;
                cout << "\nID: " << tmp->id;
                cout << "\n-----------------\n";

            }
            tmp = tmp->next;
        }
        tmp = front;
    }
    pauseConsole();
}

void sortByDuration(){

    disk *tmp = front;
    int chosenDur;

    cout << "Which duration are you interested in?\n";
    chosenDur = isInt();
    //перебираем структуру и выводим элементы,
    //длительность которых >= введеной
    while (tmp != NULL) {
        if(chosenDur <= tmp->duration){
            cout << "-----------------\n";
            cout << "NAME: " << tmp->name;
            cout << "\nYEAR: " << tmp->year;
            cout << "\nAMOUNT OF SONGS: " << tmp->amount;
            cout << "\nDURATION: " << tmp->duration;
            cout << "\nAUTHOR: " << tmp->author;
            cout << "\nID: " << tmp->id;
            cout << "\n-----------------\n";
        }

        tmp = tmp->next;
    }
    pauseConsole();
}

void earliestAlbum(){

    disk *tmp = front;

    if(front == NULL){
        cout << "\nList is empty\n";
    }

    char *chosenArt;
    int i, j, damn, n = 0;

    cout << "Which artist are you interested in?\n";
    chosenArt = isSpace();

    //считаем сколько элементов нам подходит
    while (tmp != NULL) {
        if(strcmp(chosenArt, tmp->author) == 0){
            n++;
        }
        tmp = tmp->next;
    }
    tmp = front;
    //массив со всеми годами
    int allAlbums[n];

    //собираем со всей структуры нужные года выпуска
    for(i = 0; i < amountOfDisks(front); i++){
        while (tmp != NULL) {
            if(strcmp(chosenArt, tmp->author) == 0){
                allAlbums[i] = tmp->year;
            }
            tmp = tmp->next;
        }
        tmp = front;
    }
    tmp = front;

    //сортировка пузырьком
    for(i = 0; i < n; i++){
        for(j = 0; j < n-1; j++){
            if(allAlbums[j] > allAlbums[j+1]){
                damn = allAlbums[j];
                allAlbums[j] = allAlbums[j+1];
                allAlbums[j+1] = damn;
            }
        }
    }

    //находим элемент с подходящим именем автора
    //и самым ранним годом выпуска
    while (tmp != NULL) {
        if(strcmp(chosenArt, tmp->author) == 0 && tmp->year == allAlbums[0]){
            cout << "\n-----------------\n";
            cout << "NAME: " << tmp->name;
            cout << "\nYEAR: " << tmp->year;
            cout << "\nAMOUNT OF SONGS: " << tmp->amount;
            cout << "\nDURATION: " << tmp->duration;
            cout << "\nAUTHOR: " << tmp->author;
            cout << "\nID: " << tmp->id;
            cout << "\n-----------------\n";
        }
        tmp = tmp->next;
    }
    pauseConsole();
}