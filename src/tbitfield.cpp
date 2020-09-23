// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len < 0)throw 1;
	BitLen = len;
	MemLen = ((len / 32) + 1);
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) {
		pMem[i] = 0;
	}
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) {
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	delete[]pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return(n / 32);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return pow(2, (n % 32));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0)throw 1;
	if (n > BitLen)throw - 1;
	int num;
	num = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	pMem[num] = pMem[num] | mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0)throw 1;
	if (n > BitLen)throw - 1;
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0)throw 1;
	if (n > BitLen)throw - 1;
	int num = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	return pMem[num] & mask;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	delete[]pMem;
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) {
		pMem[i] = bf.pMem[i];
	}
	return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
	if (BitLen != bf.BitLen) return false;
	for (int i = 0; i < MemLen - 1; i++) {
		if (pMem[i] != bf.pMem[i])return false;
	}
	for (int i = MemLen / 32; i < BitLen; i++) {
		if (GetBit(i) != bf.GetBit(i))return false;
	}
	return true;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
	int ost = (BitLen % 32) + 1;
	ost = pow(2, (ost));
	if (BitLen != bf.BitLen) return true;
	for (int i = 0; i < MemLen; i++) {
		if (pMem[i] != bf.pMem[i])return true;
	}
	if (((pMem[MemLen]) % ost) != ((bf.pMem[MemLen]) % ost)) return true;
	return false;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
	int ost;
	int maxBitLen;
	if (BitLen > bf.BitLen) {
		maxBitLen = BitLen;
	}
	else
	{
		maxBitLen = bf.BitLen;
	}
	TBitField temp(maxBitLen);
	for (int i = 0; i < MemLen; i++) {
		temp.pMem[i] = pMem[i] | bf.pMem[i];
	}
	return temp;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
	int len = BitLen;
	if (len < bf.BitLen)
		len = bf.BitLen;
	TBitField temp(len);
	for (int i = 0; i < MemLen; i++)
		temp.pMem[i] = pMem[i];
	for (int i = 0; i < bf.MemLen; i++)
		temp.pMem[i] &= bf.pMem[i];
	return temp;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField bf(BitLen);
	for (int i = 0; i < BitLen; i++)
		if (GetBit(i) == 0)
			bf.SetBit(i);
	return bf;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
	char c;
	int k = 0;
	do {
		istr >> c;
		if (c == '1') {
			bf.SetBit(k);
		}
		if (c == '0') {
			bf.ClrBit(k);
		}
		k++;
	} while ((c == '1') || (c == '0'));
	return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
	int i;
	for (i = 0; i < bf.BitLen; i++)
	{
		if (bf.GetBit(i) != 0) { ostr << 1; }
		else ostr << 0;
	}
	return ostr;
}