#include <iostream>
#include <string>
#include <fstream>

using namespace std;

enum Genre {
    rok,
    indi_rok,
    rep,
    phonk,
    soundtrack,
    electronica
};

string genreToString(Genre g) {
    switch (g) {
    case rok: return "рок";
    case indi_rok: return "инди-рок";
    case rep: return "рэп";
    case phonk: return "фонк";
    case soundtrack: return "саундтрек";
    case electronica: return "электронная";
    default: return "неизвестно";
    }
}

struct Track {
    string name;
    int duration;
};

struct Album {
    string title;
    string artist;
    Genre genre;
    int year;
    int total_duration;
    double price;
    int popularity;
    Track tracks[20];
    int trackCount;
};

// Функции

void printAlbum(const Album& alb) {
    cout << "==========================================" << endl;
    cout << "Альбом: " << alb.title << " | Исполнитель: " << alb.artist << endl;
    cout << "Жанр: " << genreToString(alb.genre) << " | Год: " << alb.year << " | Популярность: " << alb.popularity << endl;
    cout << "Цена: $" << alb.price << " | Общ. длина: " << alb.total_duration << " сек" << endl;
    cout << "--- Список треков (" << alb.trackCount << " шт) ---" << endl;
    for (int i = 0; i < alb.trackCount; ++i) {
        cout << "  " << i + 1 << ". " << alb.tracks[i].name << " (" << alb.tracks[i].duration << " сек)" << endl;
    }
    cout << "==========================================\n" << endl;
}

void findRock(const Album ishodniy[], int razmer, Album noviyMas[], int& schetchik) {
    schetchik = 0;
    for (int i = 0; i < razmer; i++) {
        if (ishodniy[i].genre == rok || ishodniy[i].genre == indi_rok) {
            noviyMas[schetchik] = ishodniy[i];
            schetchik++;
        }
    }
}

void sortAlbumsByArtist(Album arr[], int kolichestvo) {
    for (int i = 0; i < kolichestvo - 1; i++) {
        for (int j = 0; j < kolichestvo - i - 1; j++) {
            if (arr[j].artist > arr[j + 1].artist) {
                Album temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void printTop5(const Album mas[], int kolichestvo) {
    Album temp[20];
    for (int i = 0; i < kolichestvo; i++) temp[i] = mas[i];

    for (int i = 0; i < kolichestvo - 1; i++) {
        for (int j = 0; j < kolichestvo - i - 1; j++) {
            if (temp[j].popularity < temp[j + 1].popularity) {
                Album swap = temp[j];
                temp[j] = temp[j + 1];
                temp[j + 1] = swap;
            }
        }
    }

    cout << "\nТоп 5 альбомов по популярности:" << endl;
    cout << "==========================================\n" << endl;
    for (int i = 0; i < 5 && i < kolichestvo; i++) {
        cout << i + 1 << ". " << temp[i].artist << " - " << temp[i].title
            << " (Рейтинг популяности: " << temp[i].popularity << ")" << endl;
    }
    cout << endl;
}

void filterLongAlbums(const Album ishodniy[], int razmer, Album newMassive[], int& schetchik) {
    schetchik = 0;
    for (int i = 0; i < razmer; i++) {
        if (ishodniy[i].trackCount > 7) {
            newMassive[schetchik] = ishodniy[i];
            schetchik++;
        }
    }
}

void redactAlbum(Album& alb) {
    cout << "\nОтредактировать альбом: " << alb.title << " - " << alb.artist << endl;
    cout << "Введите новую цену без знака '$': ";
    cin >> alb.price;
    cout << "Введите новый рейтинг популярности (0-100): ";
    cin >> alb.popularity;
    cout << "Обновлено.\n" << endl;
}

// добаления доп.функции, из 10 лабораторной

void saveToBinary(const Album arr[], int kolichestvo, string imyaFayla) {
    ofstream out(imyaFayla, ios::binary);
    if (out.is_open()) {
        out.write((char*)arr, kolichestvo * sizeof(Album));
        out.close();
        cout << "Данные сохранены в файл: " << imyaFayla << endl;
    }
    else {
        cout << "Ошибка записи" << endl;
    }
}

void loadFromBinary(Album arr[], int kolichestvo, string imyaFayla) {
    ifstream in(imyaFayla, ios::binary);
    if (in.is_open()) {
        in.read((char*)arr, kolichestvo * sizeof(Album));
        in.close();
        cout << "Данные загружены из файла." << endl;
    }
    else {
        cout << "Ошибка чтения" << endl;
    }
}

void updateFromText(Album arr[], int kolichestvo, string imyaFayla) {
    ifstream in(imyaFayla);
    if (in.is_open()) {
        string iskomiyArtist;
        double novayaCena;
        int novayaPopa;

        while (getline(in, iskomiyArtist)) {
            in >> novayaCena >> novayaPopa;
            in.ignore();

            for (int i = 0; i < kolichestvo; i++) {
                if (arr[i].artist == iskomiyArtist) {
                    arr[i].price = novayaCena;
                    arr[i].popularity = novayaPopa;
                }
            }
        }
        in.close();
        cout << "Данные успешно обновлены " << endl;
    }
    else {
        cout << "Ошибка" << imyaFayla << " не найден." << endl;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    Album allAlbums[20] = {
        { "Nevermind", "Nirvana", rok, 1991, 812, 15.99, 98, {{"Smells Like Teen Spirit", 301}, {"In Bloom", 254}, {"Come as You Are", 257}}, 3 },
        { "Is This It", "The Strokes", indi_rok, 2001, 366, 12.50, 85, {{"Is This It", 154}, {"The Modern Age", 212}}, 2 },
        { "AM", "Arctic Monkeys", indi_rok, 2013, 473, 14.00, 92, {{"Do I Wanna Know?", 272}, {"R U Mine?", 201}}, 2 },
        { "The Dark Side", "Pink Floyd", rok, 1973, 795, 25.00, 100, {{"Money", 382}, {"Time", 413}}, 2 },
        { "Hot Fuss", "The Killers", indi_rok, 2004, 450, 13.00, 88, {{"Mr. Brightside", 222}, {"Somebody Told Me", 228}}, 2 },
        { "Hellboy", "Lil Peep", rep, 2016, 692, 10.00, 88, {{"Hellboy", 177}, {"Drive By", 175}, {"OMFG", 197}, {"Star Shopping", 143}}, 4},
        { "Come Over", "Lil Peep", rep, 2017, 390, 15.00, 92, {{"Awful Things", 214}, {"Save That Shit", 176}}, 2 },
        { "Phonk Universe", "Kordhell", phonk, 2022, 350, 5.00, 90, {{"Murder In My Mind", 145}, {"Live Another Day", 205}}, 2 },
        { "Midnight", "Playa", phonk, 2023, 280, 4.00, 82, {{"GigaChad", 130}, {"Drift", 150}}, 2 },
        { "Meteora", "Linkin Park", rok, 2003, 1420, 20.00, 95, {{"Foreword", 13}, {"Don't Stay", 187}, {"Somewhere I Belong", 213}, {"Lying from You", 175}, {"Hit the Floor", 164}, {"Easier to Run", 204}, {"Faint", 162}, {"Figure.09", 197}}, 8 },
        { "Forces", "Susumu Hirasawa", soundtrack, 1997, 400, 15.00, 80, {{"Forces", 240}, {"Earth", 160}}, 2 },
        { "Death or Kiss", "Kenichiro", soundtrack, 2016, 350, 25.00, 85, {{"Requiem of Silence", 200}, {"Stay Alive", 150}}, 2 },
        { "Inferno", "Mrs. Green Apple", soundtrack, 2019, 380, 20.00, 86, {{"Inferno", 210}, {"Spark", 170}}, 2 },
        { "Volume Alpha", "C418", electronica, 2011, 460, 10.00, 99, {{"Subwoofer Lullaby", 208}, {"Minecraft", 254}}, 2 },
        { "Automation", "Daniel Taylor", electronica, 2016, 450, 12.00, 80, {{"Solar Power", 220}, {"Research", 230}}, 2 },
        { "Abbey Road", "The Beatles", rok, 1969, 580, 30.00, 97, {{"Come Together", 259}, {"Something", 321}}, 2 },
        { "Franz Ferdinand", "Franz Ferdinand", indi_rok, 2004, 430, 11.00, 82, {{"Take Me Out", 237}, {"The Dark of the Matinee", 193}}, 2 },
        { "OK Computer", "Radiohead", rok, 1997, 600, 16.00, 94, {{"Paranoid Android", 383}, {"Karma Police", 217}}, 2 },
        { "Drift Tape", "Ghostface Playa", phonk, 2021, 1050, 8.00, 85, {{"Why Not", 165}, {"Swag", 120}, {"Killa", 110}, {"Grave", 100}, {"Night", 105}, {"Demon", 115}, {"Blood", 100}, {"Speed", 120}, {"Race", 115}}, 9 },
        { "A Night at the Opera", "Queen", rok, 1975, 500, 18.00, 96, {{"Bohemian Rhapsody", 354}, {"Love of My Life", 146}}, 2 }
    };

    int totalAlbums = 20;

    Album rockAlbums[20];
    int rockCount = 0;
    findRock(allAlbums, totalAlbums, rockAlbums, rockCount);

    sortAlbumsByArtist(rockAlbums, rockCount);

    cout << "Отсортированные рок и инди-рок альбомы" << endl;
    for (int i = 0; i < rockCount; i++) {
        cout << rockAlbums[i].artist << " - " << rockAlbums[i].title << " (" << genreToString(rockAlbums[i].genre) << ")" << endl;
    }
    cout << endl;

    cout << "Данные конкретного альбома" << endl;
    printAlbum(allAlbums[9]);

    printTop5(allAlbums, totalAlbums);

    Album longAlbums[20];
    int longCount = 0;
    filterLongAlbums(allAlbums, totalAlbums, longAlbums, longCount);

    cout << "Альбомы в которых 7+ песен" << endl;
    for (int i = 0; i < longCount; i++) {
        cout << longAlbums[i].artist << " - " << longAlbums[i].title << " (Треков: " << longAlbums[i].trackCount << ")" << endl;
    }


    cout << "\n Изменения альбомов" << endl;

    saveToBinary(allAlbums, totalAlbums, "albums_data.bin");


    updateFromText(allAlbums, totalAlbums, "updates.txt");

    cout << "Альбомы после обновления из файла:" << endl;
    printAlbum(allAlbums[0]); 

    printAlbum(allAlbums[5]);

    return 0;
}
