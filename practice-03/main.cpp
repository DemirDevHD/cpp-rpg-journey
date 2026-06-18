#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

struct Varlik
{
	string isim;
	float can;
	float saldiri;
	float zirh;

	void varlikOlustur(string isim,float can,float saldiri,float zirh){
		this->isim = isim;
		this->can = can;
		this->saldiri = saldiri;
		this->zirh = zirh;
	}void bilgileriYazdir() {
		cout << "-----------------------" << endl;
		cout << "Isim   : " << isim << endl;
		cout << "Can    : " << can << endl;
		cout << "Saldiri: " << saldiri << endl;
		cout << "Zirh   : " << zirh << endl;
	}
	string satirFormati() {
		return isim + "; " + to_string(can) + "; " + to_string(saldiri) + "; " + to_string(zirh);
	}
};
void dosyaOku(vector<Varlik> &varliklar,string dosyAdi) {
	ifstream dosya(dosyAdi);
	if (dosya.is_open())
	{
		varliklar.clear();
		string satir;
		getline(dosya, satir);
		while (getline(dosya, satir)) {
			stringstream ss(satir);
			string isim, can, saldiri, zirh;
			getline(ss, isim, ';');
			getline(ss, can, ';');
			getline(ss, saldiri, ';');
			getline(ss, zirh);

			Varlik yeniVarlik;
			yeniVarlik.varlikOlustur(isim, stof(can), stof(saldiri), stof(zirh));
			varliklar.push_back(yeniVarlik);
		}



	}
	else cout << "Ulasilamadi";

	dosya.close();
}
void dosyaKaydet(Varlik varlik, string dosyAdi) {
	ofstream dosya(dosyAdi, ios::app);

	if (dosya.is_open())
	{
		dosya << varlik.satirFormati() << endl;
	}




	dosya.close();
}
void varlikEkle(vector<Varlik>& varliklar, string dosyAdi) {
	string isim;float can, saldiri, zirh;
	cout << "Isim Giriniz: ";cin >> isim;
	cout << "Can Giriniz: ";cin >> can;
	cout << "Saldiri Giriniz: ";cin >> saldiri;
	cout << "Zirh Giriniz: ";cin >> zirh;

	Varlik yeni;
	yeni.varlikOlustur(isim, can, saldiri, zirh);
	varliklar.push_back(yeni);
	dosyaKaydet(yeni,dosyAdi);
}

void varlikListele(vector<Varlik> varlik){
	for (Varlik V : varlik) {
		V.bilgileriYazdir();
	}
}


int main() {
	vector<Varlik> varliklar;
	dosyaOku(varliklar, "varlik.csv");
	//dosyaKaydet("varlik.csv");
	varlikListele(varliklar);varlikEkle(varliklar, "varlik.csv");
	


}
