#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <cctype>

using namespace std;

struct User{
    string username;
    string password;
};

struct Barang{
    string pengirim;
    string alamatPengirim;
    string penerima;
    string alamatPenerima;
    string detBarang;
    string daftarBarang;
    string jumlahBarang;
};

struct Pengirim{
    string nama;
    char noTelp[20];
    string alamat;
}K;

struct Penerima{
    string nama;
    char noTelp[20];
    string alamat;
}T;

struct Detail{
    string jenis;
    int berat;
    char pulau[10];
    int jarak;
}B;


void tampilMenuUser(User user);
void updatePengiriman(Barang daftarBarang[], int& jumlahBarang);
void dataPengirim(Pengirim* K);
void dataPenerima(Penerima* T);
void detailBarang(Detail* B);
string jenisPengiriman(Detail& B);
void saveDataToFile(Pengirim& K, Penerima& T, Detail& B, const string& ekspedisi, const string& jenis);
void ekspedisi(Detail& B);

struct UserNode{
    User data;
    UserNode* next;

    UserNode(const User& userData) : data(userData), next(nullptr){}
};

class UserList{
private:
    UserNode* head;

public:
    UserList() : head(nullptr){}

    void addUser(const User& user){
        UserNode* newNode = new UserNode(user);
        newNode->next = head;
        head = newNode;
        saveToFile("user_data.txt");
    }

    bool isUsernameExists(const string& username) const{
        UserNode* current = head;
        while (current != nullptr){
            if (current->data.username == username){
                return true;
            }
            current = current->next;
        }
        return false;
    }

    bool isLoginValid(const string& username, const string& password) const {
        UserNode* current = head;
        while (current != nullptr) {
            if (current->data.username == username && current->data.password == password) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    void saveToFile(const string& filename) const{
        ofstream file(filename);
        if (file.is_open()) {
            UserNode* current = head;
            while (current != nullptr) {
                file << current->data.username << ' ' << current->data.password << '\n';
                current = current->next;
            }
            file.close();
        }
    }

    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                istringstream iss(line);
                string username, password;
                if (iss >> username >> password) {
                    User newUser = {username, password};
                    addUser(newUser);
                }
            }
            file.close();
        }
    }

    void viewQueue(const string& filename) const {
        ifstream file(filename);
        if (file.is_open()) {
            string item;
            while (getline(file, item)) {
                cout << item << endl;
            }
            file.close();
        } else {
            cout << "Gagal membuka file antrian barang." << endl;
            cout << "\n" << endl;
        }
    }

    // Mencari barang dari file
    void searchItem(const string& filename) const {
        ifstream file(filename);
        if (file.is_open()) {
            string searchItem;
            cout << "Masukkan nama barang yang dicari: ";
            cin >> searchItem;

            string item;
            bool found = false;
            while (getline(file, item)) {
                if (item.find(searchItem) != string::npos) {
                    cout << "Barang ditemukan: " << item << endl;
                    found = true;
                }
            }

            if (!found) {
                cout << "Barang tidak ditemukan." << endl;
            }

            file.close();
        } else {
            cout << "Gagal membuka file pencarian barang." << endl;
            cout << "\n" << endl;
        }
    }

};

void userMenu(UserList& userList, queue<string>& userQueue) {
    int userChoice;
    string username, password;

    const int maxUsers = 100;
    const int maxAdmins = 10;

    User users[maxUsers];
    User admins[maxAdmins];

    int userCount = 0;
    int adminCount = 0;

    int choice;

    do{
        cout << "Menu User:" << endl;
        cout << "1. Registrasi" << endl;
        cout << "2. Login" << endl;
        cout << "3. Kembali ke Menu Utama" << endl;
        cout << "Pilihan Anda: "; cin >> userChoice;

        switch(userChoice){
            case 1:
                cout << "Registrasi:" << endl;
                cout << "Masukkan username: "; cin >> username;

                if (!userList.isUsernameExists(username)){
                    cout << "Masukkan password (6 digit): "; cin >> password;
                    if(password.length() == 6){
                        User newUser = {username, password};
                        userList.addUser(newUser);
                        cout << "Registrasi berhasil!" << endl << endl;
                    }
                    else{
                        cout << "Password harus 6 digit!" << endl;
                    }
                }
                else{
                    cout << "Username sudah digunakan. Registrasi gagal." << endl << endl;
                }
                break;

            case 2:
    cout << "Login:" << endl;
    cout << "Masukkan username: "; cin >> username;

    if (userList.isUsernameExists(username)) {
        cout << "Masukkan password: ";
        string enteredPassword;
        cin >> enteredPassword;

        if (userList.isLoginValid(username, enteredPassword)) {
            userQueue.push(username);
            tampilMenuUser(users[userCount - 1]);
        } else {
            cout << "Login gagal. Username atau password salah." << endl;
            cout << "\n" << endl;
        }
    } else {
        cout << "Login gagal. Username tidak ditemukan." << endl;
        cout << "\n" << endl;
    }
    break;

            case 3:
                cout << "Kembali ke Menu Utama." << endl;
                cout << "\n" << endl;
                break;

            default:
                cout << "Pilihan tidak valid. Silakan pilih 1, 2, atau 3." << endl;
                cout << "\n" << endl;
                break;
        }
    } while (userChoice != 3);
}

// Fungsi untuk menangani menu Admin
void adminMenu(UserList& userList, queue<string>& userQueue) {
    int adminChoice;
    string adminUsername, adminPassword;

    do {
        cout << "Menu Admin:" << endl;
        cout << "1. Admin J&T Express" << endl;
        cout << "2. Admin Ninja Express" << endl;
        cout << "3. Admin Wahana Logistik" << endl;
        cout << "4. Admin Gosend" << endl;
        cout << "5. Kembali ke Menu Utama" << endl;
        cout << "Pilihan Anda: ";
        cin >> adminChoice;

        switch (adminChoice) {
            case 1:
                cout << "      J&T Express       " << endl;
                cout << "------------------------" << endl;
                cout << "Masukkan username admin: "; cin >> adminUsername;
                cout << "Masukkan password admin: "; cin >> adminPassword;

                // Admin login berhasil
                if (adminUsername == "jt" && adminPassword == "jt123") {
                    cout << "Login Admin J&T Express berhasil!" << endl;
                    cout << "\n" << endl;

                    // Tambahkan username admin ke dalam queue (sebagai contoh penggunaan Queue)
                    userQueue.push(adminUsername);
                    int jtChoice;
            do {
                cout << "Menu J&T Express:" << endl;
                cout << "1. Lihat Antrian Barang" << endl;
                cout << "2. Cari Barang" << endl;
                cout << "3. Kembali ke Menu Utama" << endl;
                cout << "Pilihan Anda: ";
                cin >> jtChoice;

                switch (jtChoice) {
                    case 1:
                        userList.viewQueue("J&TExpress_data.txt");
                        break;

                    case 2:
                        userList.searchItem("J&TExpress_data.txt");
                        break;

                    case 3:
                        cout << "Kembali ke Menu Utama." << endl;
                        cout << "\n" << endl;
                        break;

                    default:
                        cout << "Pilihan tidak valid. Silakan pilih 1, 2, atau 3." << endl;
                        cout << "\n" << endl;
                        break;
                }
            } while (jtChoice != 3);
            break;
                } else {
                    cout << "Login Admin J&T Express gagal. Username atau password admin salah." << endl;
                    cout << "\n" << endl;
                }
                break;

            case 2:
                cout << "      Ninja Express     " << endl;
                cout << "------------------------" << endl;
                cout << "Masukkan username admin: "; cin >> adminUsername;
                cout << "Masukkan password admin: "; cin >> adminPassword;

                // Admin login berhasil
                if (adminUsername == "ninja" && adminPassword == "ninja123") {
                    cout << "Login Admin Ninja Express berhasil!" << endl;
                    cout << "\n" << endl;

                    // Tambahkan username admin ke dalam queue (sebagai contoh penggunaan Queue)
                    userQueue.push(adminUsername);
                    int ninjaChoice;
            do {
                cout << "Menu Ninja Express:" << endl;
                cout << "1. Lihat Antrian Barang" << endl;
                cout << "2. Cari Barang" << endl;
                cout << "3. Kembali ke Menu Utama" << endl;
                cout << "Pilihan Anda: ";
                cin >> ninjaChoice;

                switch (ninjaChoice) {
                    case 1:
                        userList.viewQueue("NinjaExpress_data.txt");
                        break;

                    case 2:
                        userList.searchItem("NinjaExpress_data.txt");
                        break;

                    case 3:
                        cout << "Kembali ke Menu Utama." << endl;
                        cout << "\n" << endl;
                        break;

                    default:
                        cout << "Pilihan tidak valid. Silakan pilih 1, 2, atau 3." << endl;
                        cout << "\n" << endl;
                        break;
                }
            } while (ninjaChoice != 3);
            break;
                } else {
                    cout << "Login Admin Ninja Express gagal. Username atau password admin salah." << endl;
                    cout << "\n" << endl;
                }
                break;

            case 3:
                cout << "     Wahana Logistik    " << endl;
                cout << "------------------------" << endl;
                cout << "Masukkan username admin: "; cin >> adminUsername;
                cout << "Masukkan password admin: "; cin >> adminPassword;

                // Admin login berhasil
                if (adminUsername == "wahanalogistik" && adminPassword == "wahanalogistik123") {
                    cout << "Login Admin Wahana Logistik berhasil!" << endl;
                    cout << "\n" << endl;

                    // Tambahkan username admin ke dalam queue (sebagai contoh penggunaan Queue)
                    userQueue.push(adminUsername);
                    int wahanaChoice;
            do {
                cout << "Menu Wahana Logistik:" << endl;
                cout << "1. Lihat Antrian Barang" << endl;
                cout << "2. Cari Barang" << endl;
                cout << "3. Kembali ke Menu Utama" << endl;
                cout << "Pilihan Anda: ";
                cin >> wahanaChoice;

                switch (wahanaChoice) {
                    case 1:
                        userList.viewQueue("WahanaLogistik_data.txt");
                        break;

                    case 2:
                        userList.searchItem("WahanaLogistik_data.txt");
                        break;

                    case 3:
                        cout << "Kembali ke Menu Utama." << endl;
                        cout << "\n" << endl;
                        break;

                    default:
                        cout << "Pilihan tidak valid. Silakan pilih 1, 2, atau 3." << endl;
                        cout << "\n" << endl;
                        break;
                }
            } while (wahanaChoice != 3);
            break;
                } else {
                    cout << "Login Admin Wahana Logistik gagal. Username atau password admin salah." << endl;
                    cout << "\n" << endl;
                }
                // Tambahkan case untuk login sebagai Admin Wahana Logistik
                break;

            case 4:
                cout << "          GoSend        " << endl;
                cout << "------------------------" << endl;
                cout << "Masukkan username admin: "; cin >> adminUsername;
                cout << "Masukkan password admin: "; cin >> adminPassword;

                // Admin login berhasil
                if (adminUsername == "gosend" && adminPassword == "gosend123") {
                    cout << "Login Admin GoSend berhasil!" << endl << endl;
                    // Tambahkan username admin ke dalam queue (sebagai contoh penggunaan Queue)
                    userQueue.push(adminUsername);
                    int gosendChoice;
            do {
                cout << "Gosend Logistik:" << endl;
                cout << "1. Lihat Antrian Barang" << endl;
                cout << "2. Cari Barang" << endl;
                cout << "3. Kembali ke Menu Utama" << endl;
                cout << "Pilihan Anda: ";
                cin >> gosendChoice;

                switch (gosendChoice) {
                    case 1:
                        userList.viewQueue("Gosend_data.txt");
                        break;

                    case 2:
                        userList.searchItem("Gosend_data.txt");
                        break;

                    case 3:
                        cout << "Kembali ke Menu Utama." << endl;
                        cout << "\n" << endl;
                        break;

                    default:
                        cout << "Pilihan tidak valid. Silakan pilih 1, 2, atau 3." << endl;
                        cout << "\n" << endl;
                        break;
                }
            } while (gosendChoice != 3);
            break;
                }
                else{
                    cout << "Login Admin GoSend gagal. Username atau password admin salah." << endl << endl;
                }
                break;

            case 5:
                cout << "Kembali ke Menu Utama." << endl << endl;
                break;

            default:
                cout << "Pilihan tidak valid. Silakan pilih 1, 2, 3, 4, atau 5!" << endl << endl;
                break;
        }
    } while (adminChoice != 5);
}

void tampilMenuUser(User user) {
    const int maxBarang = 100;
    Barang daftarBarang[maxBarang];
    int jumlahBarang = 0;

    int choice;
    do {
        cout << "========== MENU USER ==========" << endl;
        cout << "1. Kirim Barang" << endl;
        cout << "2. Update Pengiriman" << endl;
        cout << "3. Kembali ke Menu Utama" << endl;

        cout << "Masukkan pilihan Anda: ";
        cin >> choice;
        cout << "\n" << endl;

        switch (choice) {
            case 1:
                dataPengirim(&K);
                dataPenerima(&T);
                detailBarang(&B);
                ekspedisi(B);
                break;
            case 2:
                updatePengiriman(daftarBarang, jumlahBarang);
                break;
            case 3:
                cout << "Kembali ke Menu Utama." << endl;
                cout << "\n" << endl;
                break;
            default:
                cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
                cout << "\n" << endl;
        }
    } while (choice != 5);

}

void updatePengiriman(Barang daftarBarang[], int& jumlahBarang) {
    if (jumlahBarang > 0) {
        cout << "========== UPDATE PENGIRIMAN ==========" << endl;
        cout << "Daftar Barang yang Dikirim:" << endl;

        // Tampilkan daftar barang
        for (int i = 0; i < jumlahBarang; ++i) {
            cout << i + 1 << ". " << daftarBarang[i].detBarang << endl;
        }

        // Pilih nomor barang untuk diupdate
        int nomorBarang;
        cout << "Pilih nomor barang untuk diupdate: ";
        cin >> nomorBarang;

        if (nomorBarang >= 1 && nomorBarang <= jumlahBarang) {
            // Tandai barang sebagai dikirimkan atau telah sampai di lokasi tujuan
            char statusPengiriman;
            cout << "Pilih status pengiriman (K untuk dikirimkan, S untuk sampai): ";
            cin >> statusPengiriman;

            if (statusPengiriman == 'K' || statusPengiriman == 'k') {
                cout << "Barang berhasil ditandai sebagai dikirimkan." << endl;
            } else if (statusPengiriman == 'S' || statusPengiriman == 's') {
                cout << "Barang berhasil ditandai sebagai telah sampai di lokasi tujuan." << endl;
            } else {
                cout << "Pilihan status pengiriman tidak valid." << endl;
            }

            cout << "\n" << endl;
        } else {
            cout << "Nomor barang tidak valid." << endl;
            cout << "\n" << endl;
        }
    } else {
        cout << "Belum ada barang yang dikirim." << endl;
        cout << "\n" << endl;
    }
}

void tampilanAwal(){
    system("color 3");
    cout << "\t\t\t\t\t HAI (ganti)" << endl;
    cout << endl;
    cin.get();
	system("cls");
}

void loadingBar(){
    system("color 0F");
    char a = 177, b = 219;

    cout << "\n\n\n\n";
    cout << "\n\n\n\n\t\t\t\t\tTunggu sebentar ya!\n\n";
    cout << "\t\t\t\t\t";

    for(int i = 0; i < 26; i++)
        cout << a;
        cout << "\r";
        cout << "\t\t\t\t\t";

    for(int i = 0; i < 26; i++){
        cout << b;
        Sleep(100);
    }
    system("cls");
}

void keluar(){
    cout << "Terima kasih telah menggunakan aplikasi kami (ganti)" << endl;
    exit(0);
}

void dataPengirim(Pengirim* K){
    cin.ignore();
    cout << "================================================" << endl;
    cout << "                 Data Pengirim                  " << endl;
    cout << "================================================" << endl;
    cout << "Nama            : "; getline(cin, K->nama);
    cout << "Nomor Telepon   : "; cin >> K->noTelp;
    cout << "Alamat          : "; cin.ignore(); getline(cin, K->alamat);
    system("cls");
}

void dataPenerima(Penerima* T) {
    cin.ignore();
    cout << "===============================================" << endl;
    cout << "                 Data Penerima                 " << endl;
    cout << "===============================================" << endl;
    cout << "Nama          : "; getline(cin, T->nama);
    cout << "Nomor Telepon : "; cin >> T->noTelp;
    cout << "Alamat        : "; cin.ignore(); getline(cin, T->alamat);
    system("cls");
}

void detailBarang(Detail* B) {
    cin.ignore();
    cout << "===============================================" << endl;
    cout << "                 Detail Barang                 " << endl;
    cout << "===============================================" << endl;
    cout << "Jenis Barang                : "; getline(cin, B->jenis);
    cout << "Berat Barang (dalam kg)     : "; cin >> B->berat;
    do{
        cout << "Pengiriman luar/dalam Jawa? : "; cin >> B->pulau;
        if (strcmp(strlwr(B->pulau), "luar") != 0 && strcmp(strlwr(B->pulau), "dalam") != 0){
            cout << "Masukkan pilihan luar/dalam!!" << endl;
        }
    }
    while(strcmp(strlwr(B->pulau), "luar") != 0 && strcmp(strlwr(B->pulau), "dalam") != 0);
    cout << "Jarak Pengiriman (dalam km) : "; cin >> B->jarak;
    system("cls");
}


string jenisPengiriman(Detail& B){
    int pilih;
    string jenis;
    do {
        cout << "========================================" << endl;
        cout << "         Pilih Jenis Pengiriman         " << endl;
        cout << "========================================" << endl;
        cout << "1. Same day" << endl;
        cout << "2. Next day" << endl;
        cout << "3. Reguler" << endl;
        cout << "4. Kargo" << endl;
        cout << "5. Kembali" << endl;
        cout << "6. Keluar" << endl;
        cout << "========================================" << endl;
        cout << "Masukkan pilihan: "; cin >> pilih;

        switch (pilih) {
        case 1: {
            if (strcmp(strlwr(B.pulau), "dalam") == 0) {
                if (B.berat <= 20 && B.jarak <= 200) {
                        return "Same Day";
                } else if (B.berat > 20) {
                    cout << "Jenis pengiriman tidak didukung, silahkan pilih kargo." << endl;
                } else {
                    cout << "Jenis pengiriman tidak didukung, silahkan pilih jenis pengiriman lain." << endl;
                }
            } else {
                cout << "Jenis pengiriman tidak didukung, silahkan pilih jenis pengiriman lain." << endl;
            }
            break;
        }
        case 2: {
            if (strcmp(strlwr(B.pulau), "dalam") == 0) {
                if (B.berat <= 20 && B.jarak <= 400) {
                    return "Next Day";
                } else if (B.berat > 20) {
                    cout << "Jenis pengiriman tidak didukung, silahkan pilih kargo." << endl;
                } else {
                    cout << "Jenis pengiriman tidak didukung, silahkan pilih jenis pengiriman lain." << endl;
                }
            } else {
                if (B.jarak <= 250 && B.berat <= 20) {
                    return "Next Day";
                } else if (B.berat > 20) {
                    cout << "Berat barang melebihi batas maksimal, silahkan pilih kargo." << endl;
                } else {
                    cout << "Jenis pengiriman tidak didukung, silahkan pilih jenis pengiriman lain." << endl;
                }
            }
            break;
        }
        case 3: {
            if (B.berat <= 20) {
                return "Regular";
            } else {
                cout << "Berat barang melebihi batas maksimal, silahkan pilih kargo." << endl;
            }
            break;
        }
        case 4: {
            if (B.berat >= 5) {
                return "Kargo";
            } else {
                cout << "Berat barang kurang dari batas minimum, silahkan pilih jenis pengiriman lain." << endl;
            }
            break;
        }
        case 5:
            break;
        case 6:
            keluar();
            break;
        }
    } while (pilih != 5);
}

void saveDataToFile(Pengirim& K, Penerima& T, Detail& B, const string& ekspedisi, const string& jenis) {
    ofstream outFile(ekspedisi + "_data.txt", ios::app);

    if (outFile.is_open()){
        outFile << "Jenis Pengiriman: " << jenis << endl;
        outFile << "Data Pengirim" << endl;
        outFile << "Nama: " << K.nama << endl;
        outFile << "Nomor Telepon: " << K.noTelp << endl;
        outFile << "Alamat: " << K.alamat << endl;

        outFile << "\nData Penerima" << endl;
        outFile << "Nama: " << T.nama << endl;
        outFile << "Nomor Telepon: " << T.noTelp << endl;
        outFile << "Alamat: " << T.alamat << endl;

        outFile << "\nDetail Barang" << endl;
        outFile << "Jenis Barang: " << B.jenis << endl;
        outFile << "Berat Barang: " << B.berat << " kg" << endl;
        outFile << "Jarak Pengiriman: " << B.jarak << " km" << endl;
        outFile << "=================================================" << endl << endl;

        outFile.close();
        cout << "Data berhasil disimpan di file " << ekspedisi + "_data.txt" << endl;
    } else {
        cout << "Gagal membuka file untuk penyimpanan data." << endl;
    }
}

void ekspedisi(Detail& B){
    int pilih;
    string jenis;
    do{
        cout << "===============================================" << endl;
        cout << "              Ekspedisi Pengiriman             " << endl;
        cout << "===============================================" << endl;
        cout << "1. J&T Express" << endl;
        cout << "2. Ninja Express" << endl;
        cout << "3. Wahana Logistik" << endl;
        cout << "4. Instant (Gosend)" << endl;
        cout << "5. Kembali" << endl;
        cout << "6. Keluar" << endl;
        cout << "===============================================" << endl;
        cout << "Masukkan pilihan: "; cin >> pilih;
        switch (pilih){
        case 1:
            jenis = jenisPengiriman(B);
            saveDataToFile(K, T, B, "J&TExpress", jenis);
            break;
        case 2:
            jenis = jenisPengiriman(B);
            saveDataToFile(K, T, B, "NinjaExpress", jenis);
            break;
        case 3:
            jenis = jenisPengiriman(B);
            saveDataToFile(K, T, B, "WahanaLogistik", jenis);
            break;
        case 4:
            if (B.berat <= 15 && B.jarak <= 50){
                jenis = "Same Day";
                saveDataToFile(K, T, B, "Instant_Gosend", jenis);
            }
            else if (B.berat > 15){
                cout << "Berat barang melebihi batas maksimal, silahkan pilih kargo." << endl;
            }
            else{
                cout << "Jenis pengiriman tidak didukung, silahkan pilih jenis pengiriman lain." << endl;
            }
            break;
        case 5:
            detailBarang(&B);
            break;
        case 6:
            keluar();
        }
    } while (pilih != 6);
}

int main() {
    int roleChoice;
    UserList userList;
    queue<string> userQueue;

    tampilanAwal();
    loadingBar();

    userList.loadFromFile("user_data.txt");

    do {
        cout << "Selamat datang!!" << endl;
        cout << "Pilih peran Anda:" << endl;
        cout << "1. User" << endl;
        cout << "2. Admin" << endl;
        cout << "3. Keluar dari Aplikasi" << endl;
        cout << "Pilihan Anda: "; cin >> roleChoice;
        system("cls");

        switch (roleChoice) {
            case 1:
                cout << "Anda masuk sebagai User." << endl << endl;
                userMenu(userList, userQueue);
                system("cls");
                break;

            case 2:
                cout << "Anda masuk sebagai Admin." << endl << endl;
                adminMenu(userList, userQueue);
                break;

            case 3:
                cout << "Keluar dari Aplikasi." << endl << endl;
                break;

            default:
                cout << "Pilihan tidak valid. Silakan pilih 1, 2, atau 3." << endl << endl;
                break;
        }
    } while (roleChoice != 3);

    userList.saveToFile("user_data.txt");

    return 0;
}

