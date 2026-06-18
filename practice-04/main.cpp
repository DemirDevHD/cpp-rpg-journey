#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>

#define basdusmanSayisi 3

using namespace std;

struct Varlik
{
	string isim;
	float can;
	float maxCan;
	float saldiri;
	float zirh;

	bool yasiyorMu = true;

	bool kritikVurus = false;
	bool savunma = false;
	bool kullaniMi;
	bool dusmanMi;

	Varlik(string i, float c, float mxc, float sal, float z, bool kul = false, bool dusMi = false) :
		isim(i), can(c), maxCan(mxc), saldiri(sal), zirh(z), kullaniMi(kul), dusmanMi(dusMi) {
	}

};
void dusmanlariSirala(vector<Varlik> v) {
	int id = 0;
	for (Varlik e : v)
	{
		if (e.yasiyorMu)
		{
			cout << "Id: " << id << " - " << e.isim << " Can: " << e.can << endl;

		}
		id++;
	}
}

float hasarHesapla(Varlik saldiran, Varlik hedef) {
	float hasar = saldiran.saldiri - hedef.zirh;
	if (hedef.savunma) hasar /= 2.0;
	if (saldiran.kritikVurus) hasar *= 2.0;
	if (hasar <= 0.0) hasar = 0.0;
	return hasar;
}
void saldir(Varlik& saldiran, Varlik& hedef) {
	if (!saldiran.yasiyorMu) return;
	float can = hedef.can;
	hedef.can -= hasarHesapla(saldiran, hedef);

	if (saldiran.kritikVurus) saldiran.kritikVurus = false;
	if (hedef.savunma) hedef.savunma = false;

	if (can == hedef.can) {
		cout << saldiran.isim << " Etkisiz Vurus" << endl;
		return;
	}
	else cout << saldiran.isim << " --> " << hedef.isim
		<< " Can: " << hedef.can << endl;

	if (hedef.can <= 0) {
		hedef.can = 0.0; hedef.yasiyorMu = false;
	}
}
void saldirMenu(Varlik& oyuncu, vector<Varlik>& dusmanlar, int& dusmanSayisi) {
	int id;
	dusmanlariSirala(dusmanlar);
	do
	{
		cout << "Hedef ID sec: ";
		cin >> id;
	} while ((id < 0 || id >= dusmanlar.size() || !dusmanlar[id].yasiyorMu));
	saldir(oyuncu, dusmanlar[id]);
	if (!dusmanlar[id].yasiyorMu) {
		dusmanSayisi--;
		cout << id << " idli dusman oldu!" << endl;
	}
}

void savun(Varlik& v) {
	v.savunma = true;
	cout << v.isim << " Savunma Acildi" << endl;
}
void kritikHasar(Varlik& v) {
	v.kritikVurus = true;
	cout << v.isim << " Kritik Acildi" << endl;
}
void canBas(Varlik& v) {
	v.can += 5;
	cout << v.isim << " +5 Can" << endl;
}

void dusmanEkle(vector<Varlik>& v, int x) {
	float c = 100.0, mxc = 250.0, sal = 8.0, zir = 2.0;
	for (int i = 0; i < x; i++)
	{
		string e = "Dusman " + to_string(i + 1);
		v.push_back(Varlik(e, c, mxc, sal, zir, false, true));
	}

}

void oyun(Varlik& oyuncu, vector<Varlik>& dusmanlar, int dusmanSayisi) {
	int tur = 1;
	cout << "Oyuna Hos Geldin!" << endl;
	char s;
	do
	{
		cout << "TUR :" << tur << " Dusman sayisi " << dusmanSayisi << endl;
		cout << "-------------------------" << endl;
		cout << "1-Saldir" << endl << "2-Savun" << endl << "3-Kritik" << endl;
		cin >> s;
		if (s == '1') saldirMenu(oyuncu, dusmanlar, dusmanSayisi);
		else if (s == '2') savun(oyuncu);
		else if (s == '3') kritikHasar(oyuncu);
		else if (s == '0') break;
		else {
			cout << "tanımsız secim" << endl;
			continue;
		}
		for (Varlik e : dusmanlar)
		{
			if (!e.yasiyorMu) continue;
			int se = rand() % 2 + 1;
			if (se == 1) saldir(e, oyuncu);
			if (se == 2) savun(e);
		}
		cout << oyuncu.isim << " Cani " << oyuncu.can << endl;
		dusmanlariSirala(dusmanlar);
		cout << "-------- Tur Sonu --------" << endl; tur++;

	} while (dusmanSayisi != 0 && oyuncu.yasiyorMu);
	if (oyuncu.yasiyorMu) cout << "Öldün Kaybettin" << endl;
	if (dusmanSayisi <= 0) cout << "Kazandin" << endl;


}

int main() {
	srand(time(NULL));
	Varlik oyuncu("Oyuncu", 100.0, 250.0, 24.0, 4.0, true);
	vector<Varlik> dusmanlar;
	int dusmanSayisi = basdusmanSayisi;
	dusmanEkle(dusmanlar, basdusmanSayisi);
	oyun(oyuncu, dusmanlar, dusmanSayisi);

	return 0;

}