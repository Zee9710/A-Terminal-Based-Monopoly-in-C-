#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <map>
#include <stack>
using namespace std;

int ChoiceLimiter(string text, int ll, int ul) {
    cout << text << endl;
    string i;
    getline(cin, i);
    
    while (i[0]-48 < ll || i[0]-48 > ul) {
if (cin.fail()){
   cin.clear();
   cin.sync();
   continue;
}
 cout << "Invalid Number! Enter a number between " << ll << " and " <<ul << "\n";
   getline(cin, i);


    }
    return i[0]-48;
}


class propertyTile;
class Tile;
class Card;

class Player {
public:
    string name;
    int money = 1500;
    int position = 1;
     bool jail = false;
    stack<Card*> getOutofJailCard;
    vector<propertyTile*> properties;
    Player(string n){
      this->name = n;
    }
    bool isBankrupt = false;
    int rollDice() const {
        srand(static_cast<unsigned int>(time(NULL)));
        int die1 = rand() % 6 + 1;
        int die2 = rand() % 6 + 1;
        cout<<"You rolled a "<< die1+die2<<endl;
        return die1+die2;
    }

    void move() {
        int roll = rollDice();
        if(position + roll > 40){
          this->money += 200;
          cout<<"You've made one round, your money increased by $200"<<endl;
        }
        
        position = (position + roll) % 40;
        if(position == 0){
          position =40;
        }
        
    }

    void sellProperty(propertyTile* property);
    void buyProperty(propertyTile* property);
    void payRent(propertyTile* property);
};


class Tile {
public:
    vector<Player*> playersOnTile;
    virtual void tileFunction(Player* player) { return; }
};

class propertyTile : public Tile {
public:
    string name;
    string color="";
    bool isBought = false;
    Player* owner = nullptr;
    int price = 0;
    int rent = 0;
    propertyTile(string n, string c, int p, int r){
      this->name=n;
      this->color = c;
      this->price=p;
      this->rent=r;

}
    propertyTile(string n, int p, int r) {
      this->name=n;
      this->price=p;
      this->rent=r;
    }
    void tileFunction(Player* player) override;
};

// Player member function implementations
void Player::sellProperty(propertyTile* property) {
    if (property->isBought && property->owner == this) {
        money += property->price;
        property->isBought = false;
        property->owner = nullptr;
     
        auto it = find(properties.begin(), properties.end(), property);
      properties.erase(it);
       cout << "Property sold! Current money: " << money << endl;
    }
}

void Player::buyProperty(propertyTile* property) {
    if (!property->isBought && money >= property->price) {
        money -= property->price;
        property->isBought = true;
        property->owner = this;
        properties.push_back(property);
      cout << "Property bought! Current money: " << money << endl;
    }
}

void Player::payRent(propertyTile* property) {
    if (property->isBought && property->owner != this) {
        money -= property->rent;
        property->owner->money += property->rent;
        cout << "Rent paid! Current money: " << money << endl;
    }
}

// propertyTile member function implementation
void propertyTile::tileFunction(Player* player) {
    cout<<"You landed on the property: "<< name<<endl;
    cout<<"Price: "<<price<< "\nRent: "<<rent<<endl;
    if (player == owner) {
        int choice = ChoiceLimiter("1. Sell\n2. Do nothing", 1, 2);
        if (choice == 1) {
            player->sellProperty(this);
        }
    } else if (!isBought && player->money >= price) {
        int choice = ChoiceLimiter("1. Buy\n2. Do nothing", 1, 2);
        if (choice == 1) {
            player->buyProperty(this);
        }
    } else if (isBought && player->money >= rent) {
        int choice = ChoiceLimiter("1. Pay rent", 1, 1);
        if (choice == 1) {
            player->payRent(this);
        }

      
    } else if (isBought && player->money < rent) {
        int choice = ChoiceLimiter("1. Sell Properties to pay Rent", 1, 1);
        if (choice == 1) {
          
            while (player->money < rent && !player->properties.empty()) {
                
                for(int i = 0; i< player->properties.size(); i++){
                  cout<< i + 1 << " "<< player->properties[i]->name<< ": $"<< player->properties[i]->price<<endl;
                }
                int choice = ChoiceLimiter("Choose:\n" ,1 , player->properties.size());
                cout<< "You have successfully sold " << player->properties[choice-1]->name << " "<<endl;
                player->sellProperty(player->properties[choice-1]);
              
            }
            if (player->money >= rent) {
                player->payRent(this);
            } else {
                player->isBankrupt = true;
                cout << "Player is bankrupt!\n";
            }
        }
    }
}

class freeParkingTile: public Tile{
  public:
  string name;
  freeParkingTile(string n){
    this->name=n;
  }
  void tileFunction(Player* player) override{
      cout << "You landed on a "<< name << endl;
  };
  
};



class jailTile: public Tile{
  public:
  string name;
  int JailTax;
  jailTile(string n, int t){
    this->name=n;
    this->JailTax=t;

  }
  void tileFunction(Player* player) override {
      cout << "You have landed on a Jail Tile" << endl;
    srand(static_cast<unsigned int>(time(NULL)));
    if (player->jail == true){ 
    if(player->money >= JailTax){
    int choice = ChoiceLimiter("1. roll dice\n 2.pay jail fee", 1,2);
    if (choice==1){
      int die1 = rand() % 6 + 1;
      int die2 = rand() % 6 + 1;
        int roll = die1+die2;
        cout<<"You rolled a "<<die1 << " "<< die2<<endl;
      if (die1 ==die2){
        player->position = (player->position + roll) % 40;
        if(player->position == 0){
          player->position =40;
        }
        player->jail = false;
        return;
      }
      else{
        cout<<"better luck next time"<<endl;
      }
    }
    else if(choice ==2) {
      player->jail = false;
      player->money -= JailTax;
      return;
      }
      }
    else if(player->money < JailTax) {
      int choice = ChoiceLimiter("1. roll dice", 1,1);
      if (choice==1){
        int die1 = rand() % 6 + 1;
        int die2 = rand() % 6 + 1;
          int roll = die1+die2;
        if (die1 ==die2){
          player->position = (player->position + roll) % 41;
          player->jail = false;
          return;
        }
        else{
          cout<<"better luck next time"<<endl;
        }
    }
    
    
  }}
    player->jail = true;
}
};


class TaxTile:public Tile{
  public:
  string name;
  int money;
  TaxTile(string n, int m){
    this->name=n;
    this->money=m;
  }
  void tileFunction(Player *player) override{
      cout << "You landed on  " << " " << 
          name << "tile" << endl;
  int choice = ChoiceLimiter("1. Pay Tax", 1, 1);
  if(choice == 1){
  if (player->money >= money){
     player->money -= money;
    cout << "You have paid $" << money<< " as tax ";
  }
  else if(player->money <= money && !player->properties.empty()){
    while (player->money < money && !player->properties.empty()) {
      player->sellProperty(player->properties.back());
    }
    if (player->money >= money) {
       player->money -= money;
      cout << "You have paid $" << money<< "as tax after selling your last property ";
    } else {
      player->isBankrupt = true;
      cout << "Player is bankrupt!\n";
    }
  }
  }
  }
};



  



class Card {
public:
    virtual void cardFunction(Player* player) = 0;
    virtual ~Card(){}
};

class CCCard : public Card{
public:
    bool give;
    int money;
string prompt;
    CCCard(bool g, int m, string p){
      give=g; money= m; prompt = p;
    }
    
    void cardFunction(Player* player) override {
        if (give){
        player->money += money;
        cout << "You have received $" << money<< " from " <<prompt<<endl;}
        else if (!give && player->money >= money){
           player->money -= money;
          cout << "You have paid $" << money<< " to " <<prompt<<endl;
        }
        else if(!give && player->money <= money && !player->properties.empty()){
          while (player->money < money && !player->properties.empty()) {
            player->sellProperty(player->properties.back());
          }
          if (player->money >= money) {
             player->money -= money;
            cout << "You have paid $" << money<< " to " <<prompt<<endl;
          } else {
            player->isBankrupt = true;
            cout << "Player is bankrupt!\n";
          
        }
    }
}
};

class CommunityChestTile: public Tile{
    public:
    string name;
    vector<Card*> Cards = { new CCCard(1, 50, "Holiday Mature Funds"), new CCCard(1, 75, "Bank Error"), new CCCard(1, 150, "Life Insurace"),
                            new CCCard(1, 100, "Services"), new CCCard(0, 50, "Government"), new CCCard(0, 75, "Beauty Contest"),
                            new CCCard(0, 100,"Xmas") };
    Card* drawCard(){
      srand(static_cast<unsigned int>(time(NULL)));
      int random = rand() % Cards.size();
      auto Card = Cards[random];
      return Card;
    }
    void tileFunction(Player* player) override {
        cout << "You landed on a Community Chest tile" << endl;
      int choice = ChoiceLimiter("1. Draw Card", 1, 1);
      if(choice == 1){
        auto card = drawCard();
        card->cardFunction(player);
      }
    }


};

class MoveCard: public Card{
  public:
  int move;
  MoveCard( int p){
     move = p;
  }
  void cardFunction(Player *player) override{
      cout<<"Position: "<<player->position<<endl;
      cout<<"Money: "<<player->money<<endl<<endl;
      cout<<"Chance card moved you " << move<< "spaces";
      player->position +=move;
    
    
  }
  


};
class ChanceTile: public Tile{
    public:
    string name;
    vector<Card*> Cards = { new MoveCard(1), new MoveCard(2), new MoveCard(3), new MoveCard(-1), new MoveCard(-2), new MoveCard(-3), };
    Card* drawCard(){
      srand(static_cast<unsigned int>(time(NULL)));
      int random = rand() % Cards.size();
      auto Card = Cards[random];
      return Card;
    }
    void tileFunction(Player* player) override {
        cout << "You landed on a Chance tile" << endl;
      int choice = ChoiceLimiter("1. Draw Card", 1, 1);
      if(choice == 1){
        auto card = drawCard();
        card->cardFunction(player);
      }
    }


};

auto Com = new CommunityChestTile();
auto Cha = new ChanceTile();
map<int, Tile*> board = { 
    {2,  new propertyTile("Mediterranean Avenue", "Purple", 60, 10)},
    {4,  new propertyTile("Baltic Avenue", "Purple", 30, 5)},

    {7,  new propertyTile("Oriental Avenue", "Sky", 100, 25)},    
    {9,  new propertyTile("Vermont Avenue", "Sky", 120, 35)},
    {10, new propertyTile("Conneticut Avenue", "Sky", 110, 30)},

    {12, new propertyTile("St.Charles Palace", "Pink", 170, 40)},
    {14, new propertyTile("States Avenue", "Pink", 160, 40)},
    {15, new propertyTile("Virginia Avenue", "Pink", 180, 50)},

    {17, new propertyTile("St.James Palace", "Orange", 150, 35)},
    {19, new propertyTile("Tennesse Avenue", "Orange", 180, 45)},
    {20, new propertyTile("New York Avenue", "Orange", 200, 55)},

    {22, new propertyTile("Kentucky Avenue", "Red", 200, 40)}, 
    {24, new propertyTile("Indiana Avenue", "Red", 220, 50)}, 
    {25, new propertyTile("Illinois Avenue", "Red", 240, 60)},

    {27, new propertyTile("Atlantic Avenue", "Yellow", 240, 50)},    
    {28, new propertyTile("Ventnor Avenue", "Yellow", 220, 60)},
    {30, new propertyTile("Marvin Gardens", "Yellow", 250, 70)}, 

    {32, new propertyTile("Pacific Avenue", "Green", 250, 50)},
    {33, new propertyTile("North Carolina Avenue", "Green", 270, 75)},
    {35, new propertyTile("Pennsylvania Avenue", "Green", 300, 100)},

    {38, new propertyTile("Park Palace", "SkyBlue", 350, 100)},
    {40, new propertyTile("Mayfair", "SkyBlue", 450, 125)},

    {6, new propertyTile("Reading Railroad","Station",200,25)},
    {16, new propertyTile("Pennsylvania Railroad","Station",200,25)},
    {26, new propertyTile("B. & O. Railroad","Station",200,25)},
    {36, new propertyTile("Shortline Railroad","Station",200,25)},

    {13, new propertyTile("Electric Company","Utility",200,25)},
    {29, new propertyTile("Water Works","Utility",200,25)},


    {1, new freeParkingTile("GO TILE")},
    {21,new freeParkingTile("Free Parking-1")},
    {31,new freeParkingTile("Free Parking-2")},

    {11, new jailTile("Jail Tile", 50)},
    {5,new TaxTile("Income Tax",200)},
    {39, new TaxTile("Luxury Tax", 100)},

    {3, Com}, 
    {18, Com},
    {34,Com}, 

    {8,Cha},
    {23,Cha},
    {37,Cha}

};
vector<Player*> players;
void PlayerLoop(Player* player){
  cout<<"---------------------------------"<<endl<<endl<<endl;
  cout<<player->name<<"'s turn"<<endl;
  if(player->jail){
    auto tile  = board[player->position];
    tile->tileFunction(player);
    if (player->jail){
      cout<<"Position: "<<player->position<<endl;
      cout<<"Money: "<<player->money<<endl<<endl;
      return;
    }
  }
  cout<<"Position: "<<player->position<<endl;
  cout<<"Money: "<<player->money<<endl<<endl;
  while(true){
  int choice = ChoiceLimiter("1. Roll dice 2. Sell 3. Trade  4.Check Owned Properties", 1,4);
  if(choice ==1){
  player->move();
  auto newTile = board[player->position];
  int position = player->position;
  
  newTile->tileFunction(player);
    cout<<"Position: "<<player->position<<endl;
    cout<<"Money: "<<player->money<<endl<<endl;
  // specifically for chance tile
  if(position != player->position){
    auto newerTile = board[player->position];
    newerTile->tileFunction(player);
    cout<<"Position: "<<player->position<<endl;
    cout<<"Money: "<<player->money<<endl<<endl;
  }
  break;
  }
  if(choice == 2){
    auto propertiesOwned = player->properties;
     if(propertiesOwned.empty()){
       cout << "You cant sell without Properties! " << endl;
       continue;
     }
    
    cout<<"Which property would you like to sell: ";

    
    for(int i = 0; i< propertiesOwned.size(); i++){
      cout<< i + 1 << " "<< propertiesOwned[i]->name<< ": $"<< propertiesOwned[i]->price<<endl;
    }
    int choice = ChoiceLimiter("Choose:\n" ,1 , propertiesOwned.size());
    cout<< "You have successfully sold " << propertiesOwned[choice-1]->name << " "<<endl;
    player->sellProperty(propertiesOwned[choice-1]);
    continue;
  }
  if(choice == 3){
    
    auto propertiesOwned = player->properties;
    cout<<"Select a player"<<endl;
    vector<Player*> temp = players;
    auto it = find(temp.begin(), temp.end(), player);
    temp.erase(it);
    for(int i = 0; i<temp.size(); i++){
      cout<< i+1<<". "<< temp[i]->name <<endl;
    }
    
    int selectp = ChoiceLimiter("Choose: ",1,temp.size());
    auto player2 = temp[selectp-1];
    auto propertiesOwned2 = player2->properties;
    if(propertiesOwned.empty() || propertiesOwned2.empty()){
      cout<<"You can't trade without properties"<<endl;
      continue;
    }
   
    cout<< "Your properties: "<<endl;
    for(int i = 0; i< propertiesOwned.size(); i++){
      cout<< i + 1 << " "<< propertiesOwned[i]->name<< ": $"<< propertiesOwned[i]->price<<endl;
    }
    cout<<endl<<endl;
     cout<<"Which property would you like to trade with " << player2->name << endl;
    cout<< player2->name <<" properties: "<<endl;
    for(int i = 0; i< propertiesOwned2.size(); i++){
      cout<< i + 1 << " "<< propertiesOwned2[i]->name<< ": $"<< propertiesOwned2[i]->price<<endl;
      
    }
    
    int choice = ChoiceLimiter("Choose your property:\n" ,1 , propertiesOwned.size());
    int choice2 = ChoiceLimiter("Choose Player 2's property:\n" ,1 , propertiesOwned2.size());
    int accept = ChoiceLimiter("Player 2, Do you \n1.accept?\n 2.Decline?:\n" ,1 , 2);
    if(accept == 1){
      auto property2 = propertiesOwned2[choice2 -1];
      auto property = propertiesOwned[choice -1];
      property2->owner = player;
      player->properties.push_back(property2);
        
        // Remove property from player's properties vector
        auto it = find(player->properties.begin(), player->properties.end(), property);
      player->properties.erase(it);
      property->owner = player2;
      player2->properties.push_back(property);

      // Remove property from player's properties vector
      auto it2 = find(player2->properties.begin(), player2->properties.end(), property2);
      player2->properties.erase(it2);
    cout<<"Successfully traded "<< property->name << " for "<< property2->name<<endl;
    }
    else if(accept == 2){
      cout<< player2->name<< " didn't accept"<<endl;
      continue;
    }
  }
    if(choice==4){
      auto propertiesOwned = player->properties;
      cout << "Owned Propeties: " << endl;
      if(!propertiesOwned.empty()){
      for(int i = 0; i< propertiesOwned.size(); i++){
        cout<< i + 1 << " "<< propertiesOwned[i]->name<< ": $"<< propertiesOwned[i]->price<< " " << propertiesOwned[i]->rent << endl;
      }
      }
    else {
      cout << "You have no properties" << endl;
    }

      continue;
    }
    
  }
   cout<<"---------------------------------"<<endl<<endl<<endl;
  
}
void drawBoard() {
    cout << "|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|" << endl;
    cout << "|  Free   |Kentucky | Chance  | Indiana |Illinois | B. & O. |Atlantic | Ventnor |  Water  |  Marvin |  Free   |" << endl;
    cout << "| Parking | Avenue  |    ?    |  Avenue | Avenue  |Railroad | Avenue  |  Avenue |  Works  | Gardens | Parking |" << endl;
    cout << "|         |         |         |         |         |         |         |         |         |         |         |" << endl;
    cout << "|         |   RED   |         |   RED   |   RED   |         |  YELLOW |  YELLOW | UTILITY | YELLOW  |         |" << endl;
    cout << "|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|" << endl;
    cout << "| New York|                                                                                         | Pacific |" << endl;
    cout << "| Avenue  |                                                                                         |  Avenue |" << endl;
    cout << "|         |                                                                                         |         |" << endl;
    cout << "|  ORANGE |                                                                                         |  GREEN  |" << endl;
    cout << "|---------|                                                                                         |---------|" << endl;
    cout << "|Tennessee|                                                                                         |  North  |" << endl;
    cout << "|  Avenue |                                                                                         |Carolina |" << endl;
    cout << "|         |                                                                                         |  Avenue |" << endl;
    cout << "|  ORANGE |                                                                                         |  GREEN  |" << endl;
    cout << "|---------|                                                                                         |---------|" << endl;
    cout << "|Community|                                                                                         |Community|" << endl;
    cout << "|  Chest  |                                                                                         |  Chest  |" << endl;
    cout << "|         |                                                                                         |         |" << endl;
    cout << "|         |                                                                                         |         |" << endl;
    cout << "|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|" << endl;
    cout << "|St.James |                                                                                         |Pennsylva|" << endl;
    cout << "| Place   |                                                                                         |  -nia   |" << endl;
    cout << "|         |                                                                                         |  Avenue |" << endl;
    cout << "| ORANGE  |                                __  __                               _                   |  GREEN  |" << endl;
    cout << "|---------|                               |  \\/  |                             | |                  |---------|" << endl;
    cout << "|Pennsylva|                               | \\  / | ___  _ __   ___  _ __   ___ | |_   _             |ShortLine|" << endl;
    cout << "| -nia    |                               | |\\/| |/ _ \\| '_ \\ / _ \\| '_ \\ / _ \\| | | | |            | Railroad|" << endl;
    cout << "|Railroad |                               | |  | | (_) | | | | (_) | |_) | (_) | | |_| |            |         |" << endl;
    cout << "|         |                               |_|  |_|\\___/|_| |_|\\___/| .__/ \\___/|_|\\__, |            |         |" << endl;
    cout << "|---------|                                                     | |             __/ |               |---------|" << endl;
    cout << "|Virginia |                                                     |_|            |___/                |  Chance |" << endl;
    cout << "| Avenue  |                                                                                         |    ?    |" << endl;
    cout << "|         |                                                                                         |         |" << endl;
    cout << "|   PINK  |                                                                                         |         |" << endl;
    cout << "|---------|                                                                                         |---------|" << endl;
    cout << "|  States |                                                                                         |  Park   |" << endl;
    cout << "|  Avenue |                                                                                         |  Place  |" << endl;
    cout << "|         |                                                                                         |         |" << endl;
    cout << "|   PINK  |                                                                                         |   BLUE  |" << endl;
    cout << "|---------|                                                                                         |---------|" << endl;
    cout << "|         |                                                                                         |   Tax   |" << endl;
    cout << "| UTILITY |                                                                                         |         |" << endl;
    cout << "|---------|                                                                                         |---------|" << endl;
    cout << "|   St.   |                                                                                         |Boardwalk|" << endl;
    cout << "| Charles |                                                                                         |         |" << endl;
    cout << "|  Place  |                                                                                         |         |" << endl;
    cout << "|   PINK  |                                                                                         |   BLUE  |" << endl;
    cout << "|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|" << endl;
    cout << "|  Jail   |Connectic| Vermont |  Chance | Oriental| Reading | Income  | Baltic  |Community|Mediterra|   Go    |" << endl;
    cout << "|         |   -ut   |  Avenue |    ?    |  Avenue | Railroad|  Tax    | Avenue  |  Chest  |  -nean  | Collect |" << endl;
    cout << "|         |  Avenue |         |         |         |         |         |         |         | Avenue  |  $200   |" << endl;
    cout << "|         |   SKY   |   SKY   |         |   SKY   |         |         |  PURPLE |         |  PURPLE |         |" << endl;
    cout << "|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|---------|" << endl;
}



int main() {


  
  int num = ChoiceLimiter("Enter the number of players (2-4): \n", 2,4);
  for(int i = 0; i<num; i++){
    string a;
    cout<<"Enter the name of player "<< i+1 << ": ";
    cin >> a;
    players.push_back(new Player(a));
  }
  int turn = 0;
  while(true){
    turn = turn%players.size();
    auto currentplayer = players[turn];
    drawBoard();
    PlayerLoop(currentplayer);
    if(currentplayer->isBankrupt){
      auto it = find(players.begin(), players.end(), currentplayer);
      players.erase(it);
    }
    if(players.size() ==1){
      cout<<players[0]->name<<" has won"<<endl;
      auto comchest = dynamic_cast<CommunityChestTile*>(board[3]);
      auto chance = dynamic_cast<ChanceTile*>(board[8]);
      for(auto card: comchest->Cards){
        delete card;
      }
      for(auto card: chance->Cards){
        delete card;
      }
      for(auto tile : board){
        delete tile.second;
      }
      for(auto player : players){
        delete player;
      }
      return 0;
    }
    turn++;
    }
    
    
  return 0;
}
