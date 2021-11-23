#include "Math.h"

Vector::Vector()
{

}

Vector::Vector( double xVal, double yVal )
{
	Init( xVal, yVal );
}

void Vector::Init( double xVal, double yVal )
{
	x = xVal;
	y = yVal;
}

Vector Vector::operator-( void ) const
{
	Vector output( -x, -y );
	return output;
}

Vector Vector::operator+( const Vector &vecOther ) const
{
	Vector output( x + vecOther.x, y + vecOther.y );
	return output;
}

Vector &Vector::operator+=( const Vector &vecOther )
{
	x += vecOther.x;
	y += vecOther.y;
	return *this;
}

Vector Vector::operator-( const Vector &vecOther ) const
{
	Vector output( x - vecOther.x, y - vecOther.y );
	return output;
}

Vector &Vector::operator-=( const Vector &vecOther )
{
	x -= vecOther.x;
	y -= vecOther.y;
	return *this;
}

Vector Vector::operator*( const double dOther ) const
{
	Vector output( *this );
	output.x *= dOther;
	output.y *= dOther;
	return output;
}

Vector &Vector::operator*=( const double dOther )
{
	x *= dOther;
	y *= dOther;
	return *this;
}

Vector Vector::operator/( const double dOther ) const
{
	Vector output( *this );
	output.x /= dOther;
	output.y /= dOther;
	return output;
}

Vector &Vector::operator/=( const double dOther )
{
	x /= dOther;
	y /= dOther;
	return *this;
}

double Vector::operator*( const Vector &vecOther ) const
{
	return UTIL_Dot( *this, vecOther );
}

bool Vector::operator==( const Vector &vecOther ) const
{
	return x == vecOther.x && y == vecOther.y;
}

bool Vector::operator!=( const Vector &vecOther ) const
{
	return x != vecOther.x || y != vecOther.y;
}

bool Vector::operator>( const Vector &vecOther ) const
{
	if (x == vecOther.x)
		return y > vecOther.y;
	if (y == vecOther.y)
		return x > vecOther.x;

	return x > vecOther.x && y > vecOther.y;
}

bool Vector::operator<( const Vector &vecOther ) const
{
	if (x == vecOther.x)
		return y < vecOther.y;
	if (y == vecOther.y)
		return x < vecOther.x;
	
	return x < vecOther.x && y < vecOther.y;
}

void UTIL_Normalize( Vector &vec )
{
	double dMagnitude = UTIL_Magnitude( vec );
	if (dMagnitude != 0.0)
		vec /= UTIL_Magnitude( vec );
}

double UTIL_Magnitude( const Vector &vec )
{
	return sqrt( UTIL_SqrMagnitude( vec ) );
}

double UTIL_SqrMagnitude( const Vector &vec )
{
	return vec.x * vec.x + vec.y * vec.y;
}

double UTIL_Dist( const Vector &vec1, const Vector &vec2 )
{
	Vector vecDifference = vec2 - vec1;
	return UTIL_Magnitude( vecDifference );
}

double UTIL_DistSqr( const Vector &vec1, const Vector &vec2 )
{
	Vector vecDifference = vec2 - vec1;
	return UTIL_SqrMagnitude( vecDifference );
}

void UTIL_Rotate( Vector &vec, double dAngle )
{
	double dSinAngle = sin( dAngle );
	double dCosAngle = cos( dAngle );

	Vector vecNew;
	vecNew.x = vec.x * dCosAngle - vec.y * dSinAngle;
	vecNew.y = vec.x * dSinAngle + vec.y * dCosAngle;

	vec = vecNew;
}

void UTIL_Rotate( Vector &vec, double dAngle, const Vector &vecAbout )
{
	vec -= vecAbout;
	UTIL_Rotate( vec, dAngle );
	vec += vecAbout;
}

void UTIL_Lerp( Vector &vec, const Vector &vecMin, const Vector &vecMax, double dAmount )
{
	vec = vecMin * (1.0 - dAmount) + vecMax * dAmount;
}

void UTIL_Perpendicular( Vector &vec, const Vector &vecOther )
{
	vec.x = -vecOther.y;
	vec.y = vecOther.x;
}

double UTIL_Angle( const Vector &vec1, const Vector &vec2 )
{
	return atan2( vec2.y, vec2.x ) - atan2( vec1.y, vec1.x );
}

double UTIL_Dot( const Vector &vec1, const Vector &vec2 )
{
	return vec1.x * vec2.x + vec1.y * vec2.y;
}

bool UTIL_IsWithin( const Vector &vecMin, const Vector &vecMax, const Vector &vecPoint )
{
	return vecPoint.x > vecMin.x && vecPoint.y > vecMin.y && vecPoint.x < vecMax.x && vecPoint.y < vecMax.y;
}

bool UTIL_IsWithin( double dTest, double dLow, double dHigh )
{
	return dTest > dLow && dTest < dHigh;
}

bool UTIL_IsWithinOrderless( double dTest, double dBound1, double dBound2 )
{
	if (dBound1 < dBound2)
		return UTIL_IsWithin( dTest, dBound1, dBound2 );
	
	return UTIL_IsWithin( dTest, dBound2, dBound1 );
}

bool UTIL_IsWithinInclusive( double dTest, double dLow, double dHigh )
{
	return dTest >= dLow && dTest <= dHigh;
}

bool UTIL_IsWithinOrderlessInclusive( double dTest, double dBound1, double dBound2 )
{
	if (dBound1 < dBound2)
		return UTIL_IsWithinInclusive( dTest, dBound1, dBound2 );
	
	return UTIL_IsWithinInclusive( dTest, dBound2, dBound1 );
}

bool UTIL_IsEqualError( double dTest1, double dTest2, double dError /*= M_MAXERROR*/ )
{
	return UTIL_Abs( dTest1 - dTest2 ) < dError;
}

bool UTIL_IsEqualError( const Vector &vecTest1, const Vector &vecTest2, double dError /*= M_MAXERROR*/ )
{
	return UTIL_IsEqualError( vecTest1.x, vecTest2.x, dError ) && UTIL_IsEqualError( vecTest1.y, vecTest2.y, dError );
}

bool UTIL_LessThanOrEqualError( double dTest1, double dTest2, double dError /*= M_MAXERROR*/ )
{
	return dTest1 < dTest2 || UTIL_IsEqualError( dTest1, dTest2, dError );
}

bool UTIL_GreaterThanOrEqualError( double dTest1, double dTest2, double dError /*= M_MAXERROR*/ )
{
	return dTest1 > dTest2 || UTIL_IsEqualError( dTest1, dTest2, dError );
}

bool UTIL_IsWithinError( double dTest, double dLow, double dHigh, double dError /*= M_MAXERROR*/ )
{
	return (dTest > dLow && dTest < dHigh) || UTIL_IsEqualError( dTest, dLow, dError ) || UTIL_IsEqualError( dTest, dHigh, dError );
}

bool UTIL_IsWithinOrderlessError( double dTest, double dBound1, double dBound2, double dError /*= M_MAXERROR*/ )
{
	if (dBound1 < dBound2)
		return UTIL_IsWithinError( dTest, dBound1, dBound2, dError );
	
	return UTIL_IsWithinError( dTest, dBound2, dBound1, dError );
}

void UTIL_RoundWithinError( double &dTest, double dCompare, double dError /*= M_MAXERROR*/ )
{
	if (UTIL_IsEqualError( dTest, dCompare, dError ))
		dTest = dCompare;
}

void UTIL_RoundWithinError( Vector &vecTest, const Vector &vecCompare, double dError /*= M_MAXERROR*/ )
{
	UTIL_RoundWithinError( vecTest.x, vecCompare.x, dError );
	UTIL_RoundWithinError( vecTest.y, vecCompare.y, dError );
}

//================================================================================================================================================================
// Collision
//================================================================================================================================================================

bool UTIL_CheckCol( const Vector &vecPoint, const Vector &vecDisplacement, const Vector &vecLineP1, const Vector &vecLineP2, Vector *vecHitPoint /*= NULL*/ )
{
	if (UTIL_IsPointOnLine( vecPoint, vecLineP1, vecLineP2 ))
	{
		if (vecHitPoint)
			(*vecHitPoint) = vecPoint;

		return true;
	}

	return UTIL_LineCol( vecPoint, vecPoint + vecDisplacement, vecLineP1, vecLineP2, vecHitPoint );
}

bool UTIL_IsPointOnLine( const Vector &vecPoint, const Vector &vecLineP1, const Vector &vecLineP2 )
{
	Vector vecDifference = vecLineP1 - vecLineP2;
	double dLerpAmount = ((vecPoint - vecLineP2) * vecDifference) / UTIL_SqrMagnitude( vecDifference );

	if (!(UTIL_LessThanOrEqualError( dLerpAmount, 1.0 ) && UTIL_GreaterThanOrEqualError( dLerpAmount, 0.0 )))
		return false;

	Vector vecPointOnLine;
	UTIL_Lerp( vecPointOnLine, vecLineP2, vecLineP1, dLerpAmount );

	return UTIL_IsEqualError( vecPoint, vecPointOnLine, M_MAXBIGERROR );
}

bool UTIL_LineCol( const Vector &vecLine1P1, const Vector &vecLine1P2, const Vector &vecLine2P1, const Vector &vecLine2P2, Vector *vecHitPoint /*= NULL*/ )
{
	if (!vecHitPoint)
	{
		Vector vecTempHitPoint;
		return UTIL_LineCol( vecLine1P1, vecLine1P2, vecLine2P1, vecLine2P2, &vecTempHitPoint );
	}

	if (!UTIL_InfLineCol( vecLine1P1, vecLine1P2, vecLine2P1, vecLine2P2, vecHitPoint ))
		return false;

	return UTIL_IsWithinOrderlessError( (*vecHitPoint).x, vecLine1P1.x, vecLine1P2.x, M_MAXBIGERROR ) && UTIL_IsWithinOrderlessError( (*vecHitPoint).y, vecLine1P1.y, vecLine1P2.y, M_MAXBIGERROR ) && UTIL_IsWithinOrderlessError( (*vecHitPoint).x, vecLine2P1.x, vecLine2P2.x, M_MAXBIGERROR ) && UTIL_IsWithinOrderlessError( (*vecHitPoint).y, vecLine2P1.y, vecLine2P2.y, M_MAXBIGERROR );
}

bool UTIL_OneInfLineCol( const Vector &vecLine1P1, const Vector &vecLine1P2, const Vector &vecLine2P1, const Vector &vecLine2P2, Vector *vecHitPoint /*= NULL*/ )
{
	if (!vecHitPoint)
	{
		Vector vecTempHitPoint;
		return UTIL_LineCol( vecLine1P1, vecLine1P2, vecLine2P1, vecLine2P2, &vecTempHitPoint );
	}

	if (!UTIL_InfLineCol( vecLine1P1, vecLine1P2, vecLine2P1, vecLine2P2, vecHitPoint ))
		return false;

	return UTIL_IsWithinOrderlessError( (*vecHitPoint).x, vecLine2P1.x, vecLine2P2.x, M_MAXBIGERROR ) && UTIL_IsWithinOrderlessError( (*vecHitPoint).y, vecLine2P1.y, vecLine2P2.y, M_MAXBIGERROR );
}

bool UTIL_InfLineCol( const Vector &vecLine1P1, const Vector &vecLine1P2, const Vector &vecLine2P1, const Vector &vecLine2P2, Vector *vecHitPoint /*= NULL*/ )
{
	double dA1 = vecLine1P2.y - vecLine1P1.y;
	double dB1 = vecLine1P1.x - vecLine1P2.x;

	double dA2 = vecLine2P2.y - vecLine2P1.y;
	double dB2 = vecLine2P1.x - vecLine2P2.x;
  
	double dDeterminant = dA1 * dB2 - dA2 * dB1; 

	if (UTIL_IsEqualError( dDeterminant, 0.0 ))
		return false;

	double dC1 = dA1 * vecLine1P1.x + dB1 * vecLine1P1.y;
	double dC2 = dA2 * vecLine2P1.x + dB2 * vecLine2P1.y;

	if (vecHitPoint)
	{
		(*vecHitPoint).x = (dB2 * dC1 - dB1 * dC2) / dDeterminant;
		(*vecHitPoint).y = (dA1 * dC2 - dA2 * dC1) / dDeterminant;
	}

	return true;
}

void UTIL_ClosestPointToLine( const Vector &vecPoint, const Vector &vecLineP1, const Vector &vecLineP2, Vector *vecClosestPoint )
{
	Vector vecDirection;
	UTIL_Perpendicular( vecDirection, vecLineP2 - vecLineP1 );
	UTIL_InfLineCol( vecLineP1, vecLineP2, vecPoint, vecPoint + vecDirection, vecClosestPoint );
}

bool UTIL_CheckEdgeCases( const Vector &vecHitPoint, const Vector &vecLine1P1, const Vector &vecLine1P2, const Vector &vecLine2P1, const Vector &vecLine2P2 )
{
	if (UTIL_IsEqualError( vecHitPoint, vecLine2P1 ))
	{
		if ((vecLine1P2 - vecLine1P1) * (vecLine2P1 - vecLine2P2) < 0.0)
			return false;
	}
	else if (UTIL_IsEqualError( vecHitPoint, vecLine2P2 ))
	{
		if ((vecLine1P2 - vecLine1P1) * (vecLine2P2 - vecLine2P1) < 0.0)
			return false;
	}
	
	return true;
}

bool UTIL_LineThroughCircle( const Vector &vecOrigin, double dRadius, const Vector &vecLineP1, const Vector &vecLineP2 )
{
	Vector vecClosestPoint;
	UTIL_ClosestPointToLine( vecOrigin, vecLineP1, vecLineP2, &vecClosestPoint );

	return UTIL_DistSqr( vecOrigin, vecClosestPoint ) < dRadius;
}

#if (0)
int UTIL_GetPointsOfLineOnCircle( const Vector &vecOrigin, double dRadiusSqr, const Vector &vecLineP1, const Vector &vecLineP2, Vector *vecColP1 /*= NULL*/, Vector *vecColP2 /*= NULL*/ )
{
	Vector vecClosestPoint;
	UTIL_ClosestPointToLine( vecOrigin, vecLineP1, vecLineP2, &vecClosestPoint );

	double dDistToClosestPointSqr = UTIL_DistSqr( vecClosestPoint, vecOrigin );

	if (dDistToClosestPointSqr > dRadiusSqr)
		return 0;

	if (dDistToClosestPointSqr == dRadiusSqr)
	{
		if (!UTIL_IsWithinOrderlessInclusive( vecClosestPoint.x, vecLineP1.x, vecLineP2.x ) || !UTIL_IsWithinOrderlessInclusive( vecClosestPoint.y, vecLineP1.y, vecLineP2.y ))
			return 0;

		if (vecColP1)
			(*vecColP1) = vecClosestPoint;

		return 1;
	}

	Vector vecPointOffset = vecLineP2 - vecLineP1;
	UTIL_Normalize( vecPointOffset );
	vecPointOffset *= sqrt( dRadiusSqr - dDistToClosestPointSqr );

	Vector vecTestColP1 = vecClosestPoint - vecPointOffset;
	Vector vecTestColP2 = vecClosestPoint + vecPointOffset;

	if (!UTIL_IsWithinOrderlessInclusive( vecTestColP1.x, vecLineP1.x, vecLineP2.x ) || !UTIL_IsWithinOrderlessInclusive( vecTestColP1.y, vecLineP1.y, vecLineP2.y) )
	{
		if (!UTIL_IsWithinOrderlessInclusive( vecTestColP2.x, vecLineP1.x, vecLineP2.x ) || !UTIL_IsWithinOrderlessInclusive( vecTestColP2.y, vecLineP1.y, vecLineP2.y) )
			return 0;

		if (vecColP1)
			(*vecColP1) = vecTestColP2;

		return 1;
	}
	else if (!UTIL_IsWithinOrderlessInclusive( vecTestColP2.x, vecLineP1.x, vecLineP2.x ) || !UTIL_IsWithinOrderlessInclusive( vecTestColP2.y, vecLineP1.y, vecLineP2.y) )
	{
		if (vecColP1)
			(*vecColP1) = vecTestColP1;

		return 1;
	}

	if (vecColP1)
		(*vecColP1) = vecTestColP1;
	if (vecColP2)
		(*vecColP2) = vecTestColP2;

	return 2;
}

bool UTIL_GetAdjustedAngle( const Vector &vecOrigin, const Vector &vecEdgePoint, const Vector &vecLineP1, const Vector &vecLineP2, double &dAngle )
{
	if (dAngle == 0.0)
		return false;

	Vector vecDifference = vecEdgePoint - vecOrigin;

	Vector vecColP1;
	Vector vecColP2;
	int iPointsOnCircle = UTIL_GetPointsOfLineOnCircle( vecOrigin, UTIL_SqrMagnitude( vecDifference ), vecLineP1, vecLineP2, &vecColP1, &vecColP2 );

	if (iPointsOnCircle == 0)
		return false;

	if (iPointsOnCircle == 1)
	{
		double dNewAngle = UTIL_Angle( vecColP1 - vecOrigin, vecDifference );
		if (UTIL_Abs( dNewAngle ) < UTIL_Abs( dAngle ))
			dAngle = dNewAngle;

		return true;
	}

	double dNewAngle1 = UTIL_Angle( vecColP1 - vecOrigin, vecDifference );
	double dNewAngle2 = UTIL_Angle( vecColP2 - vecOrigin, vecDifference );

	if (UTIL_Abs( dNewAngle1 ) < UTIL_Abs( dAngle ))
		dAngle = dNewAngle1;
	if (UTIL_Abs( dNewAngle2 ) < UTIL_Abs( dAngle ))
		dAngle = dNewAngle2;

	return true;
}

bool UTIL_GetAdjustedAngle( const Vector &vecOrigin, const Vector &vecEdgePoint, const Vector &vecTestPoint, double &dAngle )
{
	if (dAngle == 0.0)
		return false;

	Vector vecDifference = vecEdgePoint - vecOrigin;
	Vector vecTestDifference = vecTestPoint - vecOrigin;

	if (UTIL_SqrMagnitude( vecDifference ) < UTIL_SqrMagnitude( vecTestDifference ))
		return false;

	double dNewAngle = UTIL_Angle( vecTestDifference, vecDifference );
	if (UTIL_Abs( dNewAngle ) < UTIL_Abs( dAngle ))
		dAngle = dNewAngle;

	return true;
}

bool UTIL_LineInAngDirection( double dAngle, const Vector &vecOrigin, const Vector &vecPoint, const Vector &vecNormal )
{
	Vector vecAngleVelocity;
	UTIL_Perpendicular( vecAngleVelocity, vecPoint - vecOrigin );
	vecAngleVelocity *= dAngle;

	return UTIL_Dot( vecAngleVelocity, vecNormal ) > 0.0;
}
#endif