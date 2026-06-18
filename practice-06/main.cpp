#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class Entity 
{
private:
	string name;
	float health;
	float maxHealth;
	float armor;
	float maxArmor;
	float attackPower;

	bool isUserStatus;
	bool isEnemyStatus;
	bool isAliveStatus = true;
	bool isDefenseStatus = false;
	bool isCriticalStrikeStatus = false;
protected:

public:
	Entity(string Name, float AttackPower,
		float Health, float MaxHealth,float Armor,float MaxArmor,
		bool IsUserStatus,bool IsEnemyStatus,bool IsAliveStatus):
		name(Name),health(Health),maxHealth(MaxHealth), armor(Armor), maxArmor(MaxArmor), attackPower(AttackPower),
		isUserStatus(IsUserStatus),isEnemyStatus(IsEnemyStatus),isAliveStatus(IsAliveStatus){}

	string getName() { return name; }
	float getHealth() { return health; }
	float getMaxHealth() { return maxHealth; }
	float getArmor() { return armor; }
	float getMaxArmor() { return maxArmor; }
	float getAttackPower() { return attackPower; } 
	bool getIsUserStatus() { return isUserStatus; }
	bool getIsEnemyStatus() { return isEnemyStatus; }
	bool getIsAliveStatus() { return isAliveStatus; }
	bool getIsDefenseStatus() { return isDefenseStatus; }
	bool getIsCriticalStrikeStatus() { return isCriticalStrikeStatus; }

	void setHealth(float mHealth) { health = mHealth; };
	void setMaxHealth(float mHealth) { maxHealth = mHealth; };
	void setMaxArmor(float mArmor) { maxArmor = mArmor; };
	void setAttackPower(float attack) { attackPower = attack; }

	void setFullHealth() { health = maxHealth; }

	void takeDamage(Entity& targetEntity) {
		float damage=attackPower;
		if (isCriticalStrikeStatus) {
			damage *= 2.0;
			isCriticalStrikeStatus = false;
		}
		if (targetEntity.getIsDefenseStatus()) {
			damage /= 2.0;
			targetEntity.isDefenseStatus = false;
		}
		damage -= targetEntity.getArmor(); 

		if (damage <= 0.0) {
			damage = 0.0;
			cout << "Etkisiz Vurus (Zirhi Delemedi!)" << endl;
		}
		targetEntity.health -= damage;
		if (targetEntity.getHealth() <= 0.0) {
			targetEntity.setHealth(0.0);
			targetEntity.kill();
			cout << targetEntity.getName() << " ," << getName() << " tarafindan olduruldu" << endl;
		}
		else
		{
			cout << name << " ," << targetEntity.name << " " << damage << 
				" Hasar Verdi. "<< targetEntity.name << " Cani: " << targetEntity.getHealth() << " oldu" << endl;
		}
	}
	void kill() { isAliveStatus = false; }
	void heal() {
		health += 12;
		if (health > maxHealth) {
			health = maxHealth;
		}
		cout << name << " Can Basti. Guncel Can: " << health << endl;
	}
	void defend() { 
		isDefenseStatus = true;
		cout << name << " Savunma Acti" << endl;
	}
	void critical() { 
		isCriticalStrikeStatus = true;
		cout << name << " Kritik Acti" << endl;
	}
};

class Player : public Entity
{
private:

protected:

public:
	Player() : Entity("Oyuncu", 20, 100, 100, 5, 5, true, false, true) {}

};

class Enemy :public Entity
{
private:

protected:

public:
	Enemy(int indexEnemy) : Entity((to_string(indexEnemy + 1) + ". Dusman"), 12, 60, 60, 2, 2, false, true, true) {}
};
	

class Scene
{
private:
	int enemyCount;
	int turn = 1;
	vector<Enemy> enemies;

public:
	Scene(int enemyCount) {
		for (size_t i = 0; i < enemyCount; i++)
		{
			enemies.push_back(Enemy(i));
		}
	}

	int getTurn() { return turn; }
	int getAliveEnemyCount() {
		int isAliveEnemyCount = 0;
		for ( Enemy& e: enemies )
		{
			if (e.getIsAliveStatus()) isAliveEnemyCount++;
		}
		return isAliveEnemyCount;
	}
	int getEnemyCount() {
		return enemies.size();
	}
	vector<Enemy>& getEnemy() { return enemies; }
	void addTurn() {
		turn++;
	}

};
class Level
{
private:
	int currentLevelNumber;
	int currentSceneNumber=0;
	Player player;
	vector<Scene> scenes;

	bool isGameOverStatus=false;

	

protected:

public:
	Level(int currentLevelNum): currentLevelNumber(currentLevelNum){
		ifstream file("GameLevels.txt");
		if (!file.is_open()) { 
			cout << "Dosya Bulunamadı" << endl; 
		}
		else {
			string line;
			int sceneNum, sceneCount = 0, enemyCount;
			while (getline(file, line))
			{
				if(line.find("Level: " + to_string(currentLevelNum)) != string::npos) {
					while (getline(file, line))
					{
						if (line.find("Level:") != string::npos) { break; }
						if (line.find("Scene:") != string::npos) {
							size_t pos = line.find(":");
							sceneNum = stoi(line.substr(pos + 1));
							sceneCount++;

							while (getline(file, line)) {
								if (line.find("EnemyCount:") != string::npos) {
									size_t pos = line.find(":");
									enemyCount = stoi(line.substr(pos + 1));
									scenes.push_back(Scene(enemyCount));
									break;
								}
							}
						}
					}
					break;
				}

			}
			file.close();
		}
	}
	
	bool getGameOver() { return isGameOverStatus; }
	void gameOver() { isGameOverStatus = true; }
	/*void levelMenu() {
		cout << "Level: " << currentLevelNumber << " Sahne: " << currentSceneNumber << "/" << scenes.size() << "" <<
			" Dusman: " << scenes[currentLevelNumber - 1].getAliveEnemyCount() << "/" << scenes[currentLevelNumber - 1].getEnemyCount() <<
			" Tur: " << scenes[currentLevelNumber-1].getTurn()  <<
			" Kritik: " << (player.getIsCriticalStrikeStatus() ? " Acik " : "Kapali") <<
			" Savunma: " << (player.getIsDefenseStatus() ? " Acik " : "Kapali") << endl;
	}*/
	void levelUp() {
		player.setFullHealth();
		currentLevelNumber++; currentSceneNumber = 0;
		cout << "Tebrikler Level Atladiniz" << endl;
		Level newLevel(currentLevelNumber);
		if (newLevel.scenes.empty())
		{
			cout << "-------------------------------------------------------" << endl;
			cout << "-----------Oyunu Kazandin Tum Leveller Bitti-----------" << endl;
			cout << "-------------------------------------------------------" << endl;
			gameOver();
			return;
		}
		this->scenes = newLevel.scenes;
	}
	void sceneUp(){
		currentSceneNumber++;
		cout << "Sahne Gecildi! Yeni dusmanlar geliyor" << endl;
	}
	void attackMenu() {
		for (Enemy& e: scenes[currentSceneNumber].getEnemy())
		{
			if (e.getIsAliveStatus())
			{
				cout << e.getName() << endl;
			}
		}int c;
		while (true)
		{
			cout << "Saldirilabilecek Hedef-Id Sec: ";
			cin >> c;
			cout << endl;

			if (cin.fail()) {
				cin.clear();
				cin.ignore(1000, '\n');
				cout << "Lutfen sadece sayi giriniz!" << endl;
				continue;
			}

			if (c < 1 || c > scenes[currentSceneNumber].getEnemy().size()) {
				cout << "Gecersiz Id! Lutfen gecerli bir dusman numarasi girin." << endl;
				continue;
			}

			if (!scenes[currentSceneNumber].getEnemy()[c - 1].getIsAliveStatus()) {
				cout << "Bu dusman zaten olu! Baska bir hedef secin." << endl;
				continue;
			}

			break;
		}
		player.takeDamage(scenes[currentSceneNumber].getEnemy()[c - 1]);
	}
	void playScene() {
		cout << "-------------------------------------------------------" << endl;
		cout << "Level: " << currentLevelNumber << " | Sahne: " << currentSceneNumber+1 << "/" <<
			scenes.size() << " | Tur: " << scenes[currentSceneNumber].getTurn() << 
			" | Kalan Dusman: " << scenes[currentSceneNumber].getAliveEnemyCount()<<
			endl;
		cout << "-------------------------------------------------------" << endl;


		int choice=-1;
		do
		{
			cout << "Secimler" << endl<< "1-Saldir" << endl << "2-Savun" << endl <<
				"3-Kritik" << endl << "4-CanBas" << endl << "0-Cikis" << endl;
			cin >> choice;
			if (cin.fail()) {
				cin.clear();
				cin.ignore(1000, '\n');
				cout << "Sadece sayi giriniz!" << endl;
				choice = -1; 
				continue;    
			}
			if (choice < 0 || choice > 4) {
				cout << "Gecersiz secim! Tekrar deneyin." << endl;
				continue;
			}
			if (choice == 1) { attackMenu(); }
			else if (choice == 2) player.defend();
			else if (choice == 3) player.critical();
			else if (choice == 4) player.heal();
			else if (choice == 0) {
				gameOver(); 
				break;
			}
			for (Enemy& e : scenes[currentSceneNumber].getEnemy())
			{
				if (e.getIsAliveStatus()) {
					e.takeDamage(player);
				}
			}
			if (!player.getIsAliveStatus())
			{
				cout << "Oldun Oyun Bitti!" << endl;
				gameOver();return;
			}
			if (scenes[currentSceneNumber].getAliveEnemyCount()<=0)
			{
				if (scenes.size() <= currentSceneNumber + 1) { levelUp(); }
				else { sceneUp(); }
				break;
			}

			scenes[currentSceneNumber].addTurn();
		} while (choice < 0 || choice>4);
	}


};



void Game() {
	Level level(1);
	do
	{
		level.playScene();
	} while (!level.getGameOver());
	
	cout << "Oyun Bitti" << endl;
}

int main() {
	Game();
}