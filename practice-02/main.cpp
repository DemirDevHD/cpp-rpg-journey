#include <iostream>

using namespace std;

struct Varlik
{
	string isim;
	float can;
	float saldiri;
	float zirh;
	bool hayattaMi = true;

	bool kritikDurumu = false;
	bool savunma = false;
	bool insanMi = false;

	void varlikOlustur(string isim, float can, float saldiri, float zirh, bool insanMi=false) {
		this->isim = isim;
		this->can = can;
		this->saldiri = saldiri;
		this->zirh = zirh;
		this->insanMi = insanMi;
	}
};

int hasarHesapla(Varlik& saldiran, Varlik& hedef) {
	float hasar = saldiran.saldiri - hedef.zirh;
	if (hasar < 0.0) hasar = 0.0;
	if (saldiran.kritikDurumu == true)
	{
		if (saldiran.savunma == true) return hasar;
		return (hasar * 2.0);
	}
	else if (saldiran.savunma == true) return (hasar / 2.0);
	return hasar;
}
void saldir(Varlik& saldiran, Varlik& hedef) {
	float hasar = hasarHesapla(saldiran, hedef);
	hedef.can -= hasar;
	if (hedef.can <= 0.0) {
		hedef.hayattaMi = false;
		hedef.can = 0;
	}
}
void hedefSec(Varlik& saldiran, Varlik hedef[]) {
	int x;
	do
	{
		cout << "Saldirilabilir Hedefler " << endl;
		for (size_t i = 0; i < 2; i++)
		{
			if (hedef[i].hayattaMi) cout << i + 1 << "|" << endl;
			else if (!hedef[i].hayattaMi) cout << "Hedef Olu baska bir hedef seciniz" << endl;
			else cout << "Boyle bir hedef bulunamadi Tekrar bir hedef giriniz..." << endl;
		}
		cin >> x;

	} while (hedef[x - 1].hayattaMi != true);
	saldir(saldiran, hedef[x - 1]);
}

void savun(Varlik& v) {
	v.savunma = true;
}
void guclen(Varlik& v) {
	v.kritikDurumu = true;
}


void durum(Varlik oyuncu, Varlik dusman[], int tur) {
	cout << "Tur: " << tur << endl;
	cout << oyuncu.isim << "Can: " << oyuncu.can << endl;
	cout << dusman[0].isim << "Can: " << dusman[0].can << " Yasiyor MU?: " << dusman[0].hayattaMi << endl;
	cout << dusman[1].isim << "Can: " << dusman[1].can << " Yasiyor MU?: " << dusman[1].hayattaMi << endl;
}
void secimYap(Varlik& oyuncu, Varlik dusman[]) {
	int secim = 0;
	while (secim != 1 && secim != 2 && secim != 3)
	{
		cout << "1 Saldir" << endl << "2 Savun" << endl << "3 Guclen" << endl;
		cin >> secim;
		if (secim == 1) hedefSec(oyuncu, dusman);
		else if (secim == 2) savun(oyuncu);
		else if (secim == 3) guclen(oyuncu);
		else cout << "Tekrar Dene..." << endl;

	}
}

void oyun(Varlik& oyuncu, Varlik dusman[]) {
	int tur = 1;
	while (oyuncu.hayattaMi && (dusman[0].hayattaMi||dusman[1].hayattaMi))
	{
		secimYap(oyuncu, dusman);
		for (int i = 0; i < 2; i++) {
			if (dusman[i].hayattaMi)
			{
				saldir(dusman[i], oyuncu);
			}

		}
		durum(oyuncu, dusman, tur);
		tur++;

	}
	if (oyuncu.hayattaMi)
	{
		cout << "Kazandin..." << endl;
	}
}

int main() {
	Varlik oyuncu, dusman[2];
	int dusmanSayisi = sizeof(dusman) / sizeof(Varlik);
	oyuncu.varlikOlustur("Oyuncu 1", 100.0, 100.0, 3.0, true);
	dusman[0].varlikOlustur("Canavar 1", 90.0, 4.0, 1.5);
	dusman[1].varlikOlustur("Canavar 2", 90.0, 4.0, 1.5);
	oyun(oyuncu, dusman);
}
