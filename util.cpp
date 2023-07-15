/*
    Copyright (C) 2011 Abraham Max Santos Ramos <amsantosr@gmail.com>

    This file is part of VizMol.

    VizMol is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    VizMol is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with VizMol.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "util.h"
#include <cmath>

#define CX 0
#define CY 1
#define CZ 2

float Util::parseFloat(const QString &str, int pos, int len)
{
    float value = -1.0;
    bool ok;

    if (pos + len <= str.length()) {
        value = str.sliced(pos, len).toFloat(&ok);
    }

    return value;
}

int Util::parseInt(const QString &str, int pos, int len)
{
    bool ok;
    int value = -1;

    if (pos <= str.length()) {
        len = qMin(len, str.length() - pos);
        value = str.sliced(pos, len).toInt(&ok);
        if (!ok)
            value = -1;
    }

    return value;
}

float Util::distance(const float p1[], const float p2[])
{
    float dx = p1[CX] - p2[CX];
    float dy = p1[CY] - p2[CY];
    float dz = p1[CZ] - p2[CZ];
    return sqrt(dx*dx + dy*dy + dz*dz);
}

float Util::vectorialModulus(const float *vec)
{
    return sqrt(vec[CX] * vec[CX] + vec[CY] * vec[1] + vec[CZ] * vec[2]);
}

float Util::vectorialAngle(const float *A, const float *B)
{
    float prod = A[CX] * B[CX] + A[CY] * B[CY] + A[CZ] * B[CZ];
    float mod1 = vectorialModulus(A);
    float mod2 = vectorialModulus(B);

    const float PI = acos(-1.0f);
    float rad = acos(prod / (mod1 * mod2));
    float deg = rad * 180 / PI;
    return deg;
}

void Util::vectorialProduct(const float *A, const float *B, float *prod)
{
    prod[CX] = A[CY] * B[CZ] - A[CZ] * B[CY];
    prod[CY] = - (A[CX] * B[CZ] - A[CZ] * B[CX]);
    prod[CZ] = A[CX] * B[CY] - A[CY] * B[CX];
}
