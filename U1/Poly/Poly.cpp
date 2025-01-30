#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include "Poly.h"

using namespace std;

Poly::Poly(): grau(-1), a(nullptr) {}

Poly::Poly(const Poly& p): grau(p.grau), a(nullptr)
{
	if (getGrau() >= 0)
	{
		a = new double[getGrau() + 1];

		for (int i = 0; i <= getGrau(); ++i) a[i] = p.getCoef(i);
	}
}

Poly::Poly(Poly&& p) noexcept: grau(p.grau), a(p.a)
{
	p.grau = -1;
	p.a = nullptr;
}

Poly::~Poly()
{
	delete[] a;
}

Poly::Poly(int g): grau(g), a(nullptr)
{
	if (getGrau() == 0)
	{
		a = new double[1];

		a[0] = 0.0;
	}

	if (getGrau() > 0)
	{
		a = new double[getGrau() + 1];

		for (int i = 0; i < getGrau(); ++i) a[i] = 0.0;

		a[getGrau()] = 1.0;
	}
}

void Poly::recriar(int g)
{
	*this = Poly(g);
}

Poly& Poly::operator=(const Poly& p)
{
	if (this != &p)
	{
		if (this->getGrau() != p.getGrau())
		{

			grau = p.getGrau();

			delete[] a;

			if (getGrau() < 0) a = nullptr;

			else a = new double[getGrau() + 1];
		}

		for (int i = 0; i <= p.getGrau(); ++i) a[i] = p.getCoef(i);
	}

	return *this;
}

Poly& Poly::operator=(Poly&& p) noexcept
{
	delete[] a;

	grau = p.getGrau();
	a = p.a;

	p.grau = -1;
	p.a = nullptr;

	return *this;
}

bool Poly::empty() const
{
	return (getGrau() < 0);
}

bool Poly::isZero() const
{
	return (getGrau() == 0 && getCoef(0) == 0.0);
}

int Poly::getGrau() const { return grau; }

double Poly::getCoef(int c) const
{
	if (c < 0 || c > grau) return 0.0;

	return a[c];
}

double Poly::operator[](int c) const { return getCoef(c); }

double Poly::getValor(double x) const
{
	if (empty()) return 0.0;

	if (getGrau() == 0) return getCoef(0);

	double sum = 0.0, aux;

	for (int i = 0; i <= getGrau(); ++i) {

		aux = pow(x, i);

		sum = sum + (getCoef(i) * aux);
	}

	return sum;
}

double Poly::operator()(double x) const { return getValor(x); }

void Poly::setCoef(int i, double c)
{
	if (i < 0 || i > getGrau())
	{
		cerr << "Erro: Indice invalido\n";
		return;
	}

	if (i == getGrau() && getGrau() != 0 && c == 0.0)
	{
		cerr << "Erro: Valor invalido\n";
		return;
	}

	a[i] = c;
}

ostream& operator<<(ostream& x, const Poly& p)
{
	if (p.getGrau() < 0) return x;

	for (int i = p.getGrau(); i >= 0; i--)
	{
		if (p.getCoef(i) == 0.0)
		{
			if (i == 0 && p.getGrau() == 0) x << p.getCoef(i);
		}

		else
		{
			if (p.getCoef(i) < 0.0) x << '-';

			else
			{
				if (i != p.getGrau()) x << '+';
			}

		if (fabs(p.getCoef(i)) != 1.0 || i == 0) x << fabs(p.getCoef(i));

			if (i != 0)
			{
				if (fabs(p.getCoef(i)) != 1.0) x << '*';

				x << 'x';

				if (i > 1)
				{
					x << '^';
					x << i;
				}
			}
		}
	}
	return x;
}

istream& operator>>(istream& x, Poly& p)
{
	if (p.empty())
	{
		cerr << "Erro: Polinomio vazio \n";
		return x;
	}

	for (int i = p.getGrau(); i >= 0; i--)
	{
		cout << "x^" << i << ":\n";
		x >> p.a[i];

		if (i == p.getGrau() && p.getGrau() != 0 && p.getCoef(i) == 0.0)
		{
			do
			{
				cout << "x^" << i << ":\n";
				x >> p.a[i];
			}

			while (p.getCoef(i) == 0.0);
		}
	}

	return x;
}

bool Poly::salvar(const string& nome_arq) const
{
	ofstream arq(nome_arq);

	if (!arq.is_open()) return false;

	arq << "POLY " << getGrau() << endl;

	if (getGrau() >= 0)
	{
		for (int i = 0; i <= getGrau(); ++i) arq << getCoef(i) << ' ';
	}

	arq.close();
	return true;
}

bool Poly::ler(const string& nome_arq)
{
	ifstream arq(nome_arq);

	if (!arq.is_open()) return false;

	string title;

	arq >> title;

	if (!arq.good() || title != "POLY")
	{
		arq.close();
		return false;
	}

	int g;

	arq >> g;

	if (!arq.good())
	{
		arq.close();
		return false;
	}

	if (g < 0)
	{
		*this = Poly();

		arq.close();
		return true;
	}

	Poly prov(g);

	for (int i = 0; i <= prov.getGrau(); ++i)
	{
			arq >> prov.a[i];

			if (!arq.good() || i == g && g != 0 && prov.getCoef(i) == 0.0)
			{
				arq.close();
				return false;
			}
	}

	*this = move(prov);

	return true;
}

Poly Poly::operator+(const Poly& p) const
{
	if (p.empty() || empty() == false && p.isZero()) return *this;

	if (empty() || isZero() && p.empty() == false) return p;

	Poly prov(max(getGrau(), p.getGrau()));

	for (int i = 0; i <= prov.getGrau(); ++i) prov.a[i] = getCoef(i) + p.getCoef(i);

	while (prov.getCoef(prov.getGrau()) == 0.0 && prov.getGrau() > 0)
	{
		Poly aux(prov.getGrau() - 1);

		for (int i = 0; i <= prov.getGrau() - 1; ++i) aux.a[i] = prov.getCoef(i);

		prov = move(aux);
	}
	return prov;
}

Poly Poly::operator-(const Poly& p) const
{
	return *this + (-p);
}

Poly Poly::operator-() const
{
	if (empty() || isZero()) return *this;

	Poly prov(getGrau());

	for (int i = 0; i <= prov.getGrau(); i++) prov.a[i] = getCoef(i) * -1;

	return prov;
}

Poly Poly::operator*(const Poly& p) const
{
	if (empty() || p.empty()) return Poly();
    if (isZero() || p.isZero()) return Poly(0);

	if (getGrau() == 0 || p.getGrau() == 0)
	{
		Poly prov(max(getGrau(), p.getGrau()));

		if (getGrau() == 0)
		{
			for (int i = 0; i <= prov.getGrau(); i++) prov.a[i] = this->a[0] * p.a[i];

			return prov;
		}

		for (int i = 0; i <= prov.getGrau(); i++) prov.a[i] = this->a[i] * p.a[0];

		return prov;
	}

	Poly prov(getGrau() + p.getGrau());

	for (int k = 0; k <= prov.getGrau(); k++) prov.a[k] = 0.0;

	for (int i = 0; i <= getGrau(); i++)
	{
		for (int j = 0; j <= p.getGrau(); j++) prov.a[i + j] = prov.getCoef(i + j) + this->getCoef(i) * p.getCoef(j);
	}
	return prov;
}

bool Poly::operator==(const Poly& p) const
{
    if (getGrau() != p.getGrau()) return false;

    for (int i=0;i<= getGrau();i++) if (getCoef(i) != p.getCoef(i)) return false;

    return true;
}

bool Poly::operator!=(const Poly& p) const
{
    return !(*this == p);
}