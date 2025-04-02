/*房东要把一个房子通过中介把房子租出去理解代理模式*/
#include<iostream>
#include<string>

class RentHouse
{
public:
    virtual void rentHouse() = 0;
};

/*房东类*/
class Landlord : public RentHouse
{
public:
    void rentHouse()
    {
        std::cout << "将房子租出去" << std::endl;
    }
};

/*中介类*/
class Intermediary : public RentHouse
{
public:
    void rentHouse()
    {
        std::cout << "发布招租启示" << std::endl;
        std::cout << "招人看房" << std::endl;
        _landlord.rentHouse();
        std::cout << "负责租后维修" << std::endl;
    }

private:
    Landlord _landlord;
};

int main()
{
    Intermediary intermediary;
    intermediary.rentHouse();
    return 0;
}