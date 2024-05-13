#include <iostream>

#ifndef POLY_H
#define POLY_H

class Poly
{
    private:

        int grau;
        double* a;

    public:

        Poly();

        Poly(const Poly& p);

        Poly(Poly&& p) noexcept;

        ~Poly();

        explicit Poly(int g);

        void recriar(int g);

        Poly& operator=(const Poly& p);

        Poly& operator=(Poly&& p) noexcept;

        bool empty() const;

        bool isZero() const;

        int getGrau() const;

        double getCoef(int c) const;

        double operator[](int c) const;

        double getValor(double x) const;

        double operator()(double x) const;

        void setCoef(int i, double c);

        friend std::ostream& operator<<(std::ostream& x, const Poly& p);

        friend std::istream& operator>>(std::istream& x, Poly& p);

        bool salvar(const std::string& nome_arq) const;

        bool ler(const std::string& nome_arq);

        Poly operator+(const Poly& p) const;

        Poly operator-(const Poly& p) const;

        Poly operator-() const;

        Poly operator*(const Poly& p) const;

        bool operator==(const Poly& p) const;

        bool operator!=(const Poly& p) const;
};

#endif