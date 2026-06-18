#include <iostream>
#include <ctime>

using namespace std;

struct Karakter
{
	string isim;
	float can = 100;
	float saldiri;
	float zirh;
	bool hayattaMi=true;

	void karakterOlustur(string isim, float can, float saldiri, float zirh) {
		this->isim = isim;
		this->can = can;
		this->saldiri = saldiri;
		this->zirh = zirh;
	}
};
//int hasarHesapla(){}
void saldir(Karakter &saldiran,Karakter &saldirilan){
	saldirilan.can -= saldiran.saldiri;
	if (saldirilan.can <= 0) {
		saldirilan.can = 0;
		saldirilan.hayattaMi = false;
		cout << saldirilan.isim << " Oldu..." <<endl;
		return;
	}
	cout << saldiran.isim << ", " << saldirilan.isim << " saldirdi yeni cani: " << saldirilan.can << endl;
}
void canBas(Karakter& k){
	k.can += 5.0;
	cout << k.isim << " Can Basti yeni cani: " << k.can << endl;
}
void secimYap(Karakter& secenKarakter,Karakter& digerKarakter){
	int secim;
	do
	{
		cout << "1 Saldir" << endl << "2 Can Bas" << endl;
		cin >> secim;
		if (secim == 1) saldir(secenKarakter,digerKarakter);
		else if (secim == 2) canBas(secenKarakter);
		else cout << "Tekrar Sec" << endl;
	} while (secim != 1 && secim != 2 && secim != 3);
}
void rSecimYap(Karakter& secenKarakter, Karakter& digerKarakter){
	int secim = rand() % 2 + 1;
	if (secim == 1) saldir(secenKarakter, digerKarakter);
	else if (secim == 2) canBas(secenKarakter);
}
void menu(Karakter o,Karakter d,int tur){
	cout << "-------------------------" << endl;
	cout << tur << ". Tur" << endl;
	cout << o.isim << " Cani " << o.can << endl;
	cout << d.isim << " Cani " << d.can << endl;
	cout << "-------------------------" << endl;
}
void oyun(Karakter &oyuncu,Karakter &dusman){
	int tur = 1;
	while (oyuncu.hayattaMi && dusman.hayattaMi)
	{
		menu(oyuncu, dusman,tur);
		secimYap(oyuncu, dusman);
		if (!dusman.hayattaMi) break;
		rSecimYap(dusman, oyuncu);
		if (!oyuncu.hayattaMi) break;
		tur++;

	}
	if (dusman.hayattaMi == false) cout << "Kazandin";
	else cout << "Kaybettin";


}




int main() {
	srand(time(0));
	Karakter oyuncu,dusman;
	oyuncu.karakterOlustur("Oyuncu", 100.0, 11.0, 5.0);
	dusman.karakterOlustur("Dusman", 120.0, 8.0, 3.0);
	oyun(oyuncu, dusman);
	
}
