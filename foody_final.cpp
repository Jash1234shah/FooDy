#include <iostream>
#include <vector>
using namespace std;

class Order {
private:
    vector<string> food_items;
    float total_price;
    string order_time;
    string driver_assigned;

public:

    Order(vector<string> items, float price, string time)
    {
        food_items = items;
        total_price = price;
        order_time = time;
        driver_assigned = "None";
    }

    float getTotalPrice()
    {
        return total_price;
    }

    void assignDriver(string driver)
    {
        driver_assigned = driver;
    }

    void displayOrder()
    {
        cout << "\nOrder Details\n";
        cout << "Items: ";

        for(string item : food_items)
        {
            cout << item << " ";
        }

        cout << "\nTotal Price: $" << total_price;
        cout << "\nOrder Time: " << order_time;
        cout << "\nDriver: " << driver_assigned << endl;
    }
};

class Account {
protected:

    string email;
    string password;
    float wallet_balance;
    string delivery_address;
    string phone_number;
    string account_created_date;

public:

    Account(string e,string p,float w,string addr,string phone,string date)
    {
        email = e;
        password = p;
        wallet_balance = w;
        delivery_address = addr;
        phone_number = phone;
        account_created_date = date;
    }

    void addFunds(float amount)
    {
        wallet_balance += amount;
    }

    void deductFunds(float amount)
    {
        wallet_balance -= amount;
    }

    float getBalance()
    {
        return wallet_balance;
    }

    void displayAccountInfo()
    {
        cout << "\nEmail: " << email;
        cout << "\nWallet Balance: $" << wallet_balance;
        cout << "\nAddress: " << delivery_address;
        cout << "\nPhone: " << phone_number;
        cout << "\nAccount Created: " << account_created_date << endl;
    }
};

class RestaurantAccount : public Account {

private:
    vector<string> menu;
    vector<float> prices;

public:

    RestaurantAccount(string e,string p,float w,string addr,string phone,string date)
    : Account(e,p,w,addr,phone,date)
    {
        menu = {"Pizza","Burger","Pasta"};
        prices = {15,10,12};
    }

    void showMenu()
    {
        cout << "\nRestaurant Menu\n";

        for(int i=0;i<menu.size();i++)
        {
            cout << i+1 << ". " << menu[i] << " - $" << prices[i] << endl;
        }
    }

    string getItemName(int choice)
    {
        if(choice>=1 && choice<=menu.size())
        return menu[choice-1];

        return "";
    }

    float getItemPrice(int choice)
    {
        if(choice>=1 && choice<=prices.size())
        return prices[choice-1];

        return -1;
    }

    void receivePayment(float amount)
    {
        wallet_balance += amount;

        cout << "Restaurant received payment: $" << amount << endl;
    }
};

class CustomerAccount : public Account {

private:
    vector<Order> order_history;
    bool is_premium_member;
    string premium_code;

public:

    CustomerAccount(string e,string p,float w,string addr,string phone,string date,bool premium,string code)
    : Account(e,p,w,addr,phone,date)
    {
        is_premium_member = premium;
        premium_code = code;
    }

    float applyPremiumCode(string enteredCode,float originalPrice)
    {
        if(!is_premium_member)
        {
            cout << "This account is not premium\n";
            return originalPrice;
        }

        if(enteredCode==premium_code)
        {
            float discount = originalPrice*0.20;
            float finalPrice = originalPrice-discount;

            cout << "\nPremium Discount Applied\n";
            cout << "Original Price: $" << originalPrice << endl;
            cout << "Discount: $" << discount << endl;
            cout << "Final Price: $" << finalPrice << endl;

            return finalPrice;
        }

        cout << "Invalid premium code\n";
        return originalPrice;
    }

    void placeOrder(Order order,RestaurantAccount &restaurant)
    {
        float total = order.getTotalPrice();

        if(wallet_balance>=total)
        {
            deductFunds(total);
            restaurant.receivePayment(total);

            order_history.push_back(order);

            cout << "Order placed successfully\n";
        }
        else
        {
            cout << "Not enough wallet balance\n";
        }
    }

    void mergeAccount(CustomerAccount &other)
    {
        wallet_balance += other.wallet_balance;

        other.wallet_balance = 0;

        cout << "Accounts merged successfully\n";
    }

    void displayAccountInfo()
    {
        Account::displayAccountInfo();

        cout << "Premium Member: ";

        if(is_premium_member)
        cout << "Yes\n";
        else
        cout << "No\n";
    }
};

class DriverAccount : public Account {

public:

    DriverAccount(string e,string p,float w,string addr,string phone,string date)
    : Account(e,p,w,addr,phone,date) {}

    void claimOrder(Order &order)
    {
        order.assignDriver(email);

        cout << "Driver claimed order\n";
    }

    void completeDelivery(Order &order)
    {
        cout << "Order delivered\n";
    }
};

int main()
{
    cout << "Welcome to FooDy Food Delivery\n";

    cout << "\nLogin as:\n1 Customer\n2 Driver\n3 Restaurant\nChoice: ";

    int role;
    cin >> role;
    cin.ignore();

    RestaurantAccount restaurant(
        "rest@foody.com","pass",0,
        "Restaurant Street","9999999999","2025"
    );

    if(role==1)
    {
        CustomerAccount customer(
            "cust@foody.com","1234",100,
            "Street A","1111111111","2025",
            true,"7749"
        );

        DriverAccount driver(
            "driver@foody.com","pass",0,
            "Driver St","8888888888","2025"
        );

        CustomerAccount customer2(
            "friend@foody.com","pass",50,
            "Street A","2222222222","2025",
            false,""
        );

        while(true)
        {
            cout << "\n1 Place Order\n2 Merge Accounts\n3 Exit\nChoice: ";
            int choice;
            cin >> choice;
            cin.ignore();

            if(choice==3) break;

            if(choice==2)
            {
                customer.mergeAccount(customer2);
                continue;
            }

            restaurant.showMenu();

            vector<string> items;
            float totalPrice=0;

            while(true)
            {
                cout << "Add item number (0 to finish): ";
                int itemChoice;
                cin >> itemChoice;

                if(itemChoice==0) break;

                string name = restaurant.getItemName(itemChoice);
                float price = restaurant.getItemPrice(itemChoice);

                if(name=="")
                {
                    cout << "Invalid item\n";
                    continue;
                }

                items.push_back(name);
                totalPrice += price;

                cout << name << " added. Total: $" << totalPrice << endl;
            }

            if(items.empty())
            {
                cout << "Order cancelled\n";
                continue;
            }

            cout << "\nSelect Payment Method\n";
            cout << "1 Wallet\n";
            cout << "2 Cancel Order\nChoice: ";

            int pay;
            cin >> pay;
            cin.ignore();

            if(pay!=1)
            {
                cout << "Order cancelled\n";
                continue;
            }

            Order order(items,totalPrice,"Now");

            customer.placeOrder(order,restaurant);

            cout << "\nCustomer Balance: $" << customer.getBalance() << endl;
            cout << "Restaurant Balance: $" << restaurant.getBalance() << endl;

            driver.claimOrder(order);
            driver.completeDelivery(order);

            order.displayOrder();
        }
    }

    else if(role==2)
    {
        DriverAccount driver(
            "driver@foody.com","pass",0,
            "Driver St","8888888888","2025"
        );

        vector<string> items={"Pizza"};

        Order order(items,20,"6PM");

        driver.claimOrder(order);
        driver.completeDelivery(order);
        order.displayOrder();
    }

    else if(role==3)
    {
        restaurant.displayAccountInfo();
        restaurant.showMenu();
    }

    return 0;
}