/*通过苹果笔记本电脑的构造理解建造者模式*/

#include <iostream>
#include <string>
#include <memory>

class Computer
{
public:
    void setBorad(const std::string &borad)
    {
        _borad = borad;
    }
    void setDisplay(const std::string &display)
    {
        _display = display;
    }

    void showParamaters()
    {
        std::string param = "Computer Paramaters:\n";
        param += "\tBorad: " + _borad + "\n";
        param += "\tDisplay: " + _display + "\n";
        param += "\tOS: " + _os + "\n";
        std::cout << param << std::endl;
    }

    virtual void setOs() = 0;

protected:
    std::string _borad;
    std::string _display;
    std::string _os;
};

class MacBook : public Computer
{
public:
    void setOs() override
    {
        _os = "Mac OS x15";
    }
};

class Builder
{
public:
    virtual void buildBorad(const std::string &borad) = 0;
    virtual void buildDisplay(const std::string &display) = 0;
    virtual void buildOs() = 0;
    virtual std::shared_ptr<Computer> build() = 0;
};

class MacBookBuilder : public Builder
{
public:
    MacBookBuilder() : _computer(new MacBook()) {}
    void buildBorad(const std::string &borad)
    {
        _computer->setBorad(borad);
    }

    void buildDisplay(const std::string &display)
    {
        _computer->setDisplay(display);
    }

    void buildOs()
    {
        _computer->setOs();
    }

    std::shared_ptr<Computer> build()
    {
        return _computer;
    }

private:
    std::shared_ptr<Computer> _computer;
};

/*指挥者类*/
class Diretor
{
public:
    Diretor(Builder *builder) : _build(builder) {}

    void construct(const std::string &borad, const std::string &display)
    {
        _build->buildBorad(borad);
        _build->buildDisplay(display);
        _build->buildOs();
    }

private:
    std::shared_ptr<Builder> _build;
};

int main()
{
    Builder *builder = new MacBookBuilder();
    std::unique_ptr<Diretor> diretor(new Diretor(builder));
    diretor->construct("华硕主板", "三星显示器");
    std::shared_ptr<Computer> computer = builder->build();
    computer->showParamaters();
    return 0;
}