#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#define levelSayisi 4

using namespace std;

class Varlik
{
public:
	string isim;
	float can;
	float maxCan;
	float saldiri;
	float zirh;

	bool hayattaMi=true;


	bool dusmanMi = true;

	bool savunma = false;
	bool kritikVurus = false;
	void savun() {
		savunma = true;
		cout << isim << " Savunma Acti" << endl;
	}
	void kritik() {
		kritikVurus = true;
		cout << isim << " Kritik Acti" << endl;
	}
};
class Oyuncu : public Varlik
{
public:
	bool kullaniciMi = true;

	Oyuncu() {
		isim = "Oyuncu";
		can = 100.0;maxCan = 120.0; saldiri = 24;zirh = 2;
	}
	void canBas() {
		can += 8;
	}
};
class Dusman : public Varlik
{
public:
	int dusmanId;
	Dusman(int x) {
		isim = "Dusman " + to_string(x); dusmanId = x - 1;
		can = 80.0;maxCan = 100.0; saldiri = 12;zirh = 2;
		
	}
};
class Level
{
public:
	bool oyunBitti =false;
	int lSeviye;
	int dusmanSayisi;
	Oyuncu oyuncu;
	vector<Dusman> Dusmanlar;
	Level(int l,int dS,Oyuncu o):lSeviye(l),dusmanSayisi(dS),oyuncu(o) {
		for (int i = 1; i <= dusmanSayisi; i++)
		{
			Dusman dusman(i); Dusmanlar.push_back(dusman);
		}
	}
	void dusmanlariSirala() {
		for (Dusman d: Dusmanlar){
			if (d.hayattaMi)
			{
				cout<< "Id: " << d.dusmanId+1 << " Isim: " << d.isim << " Cani: " << d.can << endl;
			}
		}
	}
};
Level levelOku(Level& level, int& sulevel) {
	ifstream levelDosya("Level.csv");
	if (levelDosya.is_open()) {
		string satir;
		for (int i = 0; i <= sulevel; i++)
		{
			getline(levelDosya, satir);
		}
		stringstream ss(satir);
		string kaclevel, dusmansay;
		getline(ss, kaclevel, ';');
		getline(ss, dusmansay, ';');

		Oyuncu oyuncu;
		levelDosya.close();
		return Level(stoi(kaclevel), stoi(dusmansay), oyuncu);
	}
}

float hasarHesapla(Varlik& saldiran, Varlik& hedef) {
	float hasar = saldiran.saldiri - hedef.zirh;
	if (saldiran.kritikVurus) {
		hasar *= 2.0; 
		saldiran.kritikVurus = false;
	}
	if (hedef.savunma) {
		 hasar /= 2.0; 
		 hedef.savunma = false;
	}
	if (hasar <= 0.0) hasar = 0.0;
	return hasar;
}

void saldir(Varlik& saldiran,Varlik& hedef,int& dusmanSayisi){
	float hasar = hasarHesapla(saldiran,hedef);
	hedef.can -= hasar;
	if (hedef.can <= 0.0)
	{
		hedef.can = 0.0; hedef.hayattaMi = false;
	}
	if (!hedef.hayattaMi) { 
		cout << hedef.isim << " Oldu" << endl;
		if (hedef.dusmanMi) dusmanSayisi--;
	}

	else cout << saldiran.isim << " --> " << hedef.isim << " Can: " << hedef.can << endl;
}

void saldirMenu(Level& level) {
	cout << "Saldirilacak id gir " << endl;
	level.dusmanlariSirala(); int ids;
	while(true)
	{
		cin >> ids;
		if (ids >= level.Dusmanlar.size() || ids < 0)
		{
			cout << "Tanımsız Id" << endl;
			continue;
		}
		if (!level.Dusmanlar[ids-1].hayattaMi) {
			cout << "Bu dusman zaten olu, Baska Hedef sec: " << endl;
			continue;
		}

		saldir(level.oyuncu,level.Dusmanlar[ids-1],level.dusmanSayisi); 
		break;
	}
}

void oyun(Level& level, int& suanLevel) {
	level.oyuncu.can = 120.0;int tur = 1;
	cout << "------- Level: " << suanLevel << " -------" << endl;
	do
	{
		cout << "----- Tur: " << tur << " Kalan Dusman Sayisi: " << level.dusmanSayisi << " -----" << endl;
		cout << "1-Saldir" << endl << "2-Savun" << endl
			<< "3-Kritik" << endl << "4-Canbas" << endl << "0-Cikis" << endl;
		int s;
		do
		{
			cin >> s;
			if (cin.fail()) {
				cin.clear();
				cin.ignore(1000, '\n');
				cout << "Lutfen sadece sayi giriniz!" << endl;
				continue;
			}
			if (s == 1) saldirMenu(level);
			if (s == 2) level.oyuncu.savun();
			if (s == 3) level.oyuncu.kritik();
			if (s == 4) level.oyuncu.canBas();
			if (s == 0) {
				cout << "Oyundan Cikildi" << endl;
				level.oyunBitti = true;
				return;
			}
		} while (s >= 4 || s <= 0);


		for (int i = 0; i < level.Dusmanlar.size(); i++)
		{
			if (!level.Dusmanlar[i].hayattaMi) continue;
			saldir(level.Dusmanlar[i], level.oyuncu,level.dusmanSayisi);
			if (!level.oyuncu.hayattaMi) {
				cout << "Oyunu Kaynettiin " << endl;
				level.oyunBitti = true;
				return;
			}
		}

		tur++;
	} while (level.dusmanSayisi>0);
	if (level.dusmanSayisi >= 0) suanLevel++;

}


int main() {
	int suanLevel = 1;
	cout << "Oyun basliyor" << endl;
	for (int i = 0; i < levelSayisi; i++)
	{
		Level level = levelOku(level, suanLevel);
		oyun(level, suanLevel);
		if (!level.oyuncu.hayattaMi) break;
		if (level.oyunBitti) break;
	}
	return 0;
}
