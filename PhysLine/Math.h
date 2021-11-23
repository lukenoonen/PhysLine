#ifndef MATH_H
#define MATH_H

#include <math.h>

#ifndef NULL
#define NULL nullptr
#endif // NULL

struct Vector
{
	Vector();
	Vector( double xVal, double yVal );

	void Init( double xVal, double yVal );

	Vector operator-( void ) const;
	Vector operator+( const Vector &vecOther ) const;
	Vector &operator+=( const Vector &vecOther );
	Vector operator-( const Vector &vecOther ) const;
	Vector &operator-=( const Vector &vecOther );
	Vector operator*( const double dOther ) const;
	Vector &operator*=( const double dOther );
	Vector operator/( const double dOther ) const;
	Vector &operator/=( const double dOther );
	double operator*( const Vector &vecOther ) const;
	bool operator==( const Vector &vecOther ) const;
	bool operator!=( const Vector &vecOther ) const;
	bool operator>( const Vector &vecOther ) const;
	bool operator<( const Vector &vecOther ) const;
	bool operator>=( const Vector &vecOther ) const;
	bool operator<=( const Vector &vecOther ) const;

	double x, y;
};

// Constant vectors
const Vector	vec_zero( 0.0, 0.0 );
const Vector	vec_one( 1.0, 1.0 );
const Vector	vec_up( 0, 1.0 );
const Vector	vec_down( 0.0, -1.0 );
const Vector	vec_right( 1.0, 0.0 );
const Vector	vec_left( -1.0, 0.0 );

// Math constants
#define M_MAXERROR					1.0e-12
#define M_MAXBIGERROR				1.0e-6

// Math defines
#define UTIL_Max( a, b )			((a) > (b) ? (a) : (b))
#define UTIL_Min( a, b )			((a) < (b) ? (a) : (b))
#define UTIL_Abs( a )				((a) < 0.0 ? -(a) : (a))
#define UTIL_Clamp( a, b, c )		((a) > (b) ? (b) : ((a) < (c) ? (c) : (a)))
#define UTIL_SameSign( a, b )		((a) < 0.0 ? ((b) < 0.0) : ((b) >= 0.0))

void	UTIL_Normalize( Vector &vec );
double	UTIL_Magnitude( const Vector &vec );
double	UTIL_SqrMagnitude( const Vector &vec );
double	UTIL_Dist( const Vector &vec1, const Vector &vec2 );
double	UTIL_DistSqr( const Vector &vec1, const Vector &vec2 );
void	UTIL_Rotate( Vector &vec, double dAngle );
void	UTIL_Rotate( Vector &vec, double dAngle, const Vector &vecAbout );
void	UTIL_Lerp( Vector &vec, const Vector &vecMin, const Vector &vecMax, double dAmount );
void	UTIL_Perpendicular( Vector &vec, const Vector &vecOther );
double	UTIL_Angle( const Vector &vec1, const Vector &vec2 );
double	UTIL_Dot( const Vector &vec1, const Vector &vec2 );
bool	UTIL_IsWithin( const Vector &vecMin, const Vector &vecMax, const Vector &vecPoint );
bool	UTIL_IsWithin( double dTest, double dLow, double dHigh );
bool	UTIL_IsWithinOrderless( double dTest, double dBound1, double dBound2 );
bool	UTIL_IsWithinInclusive( double dTest, double dLow, double dHigh );
bool	UTIL_IsWithinOrderlessInclusive( double dTest, double dBound1, double dBound2 );
bool	UTIL_IsEqualError( double dTest1, double dTest2, double dError = M_MAXERROR );
bool	UTIL_IsEqualError( const Vector &vecTest1, const Vector &vecTest2, double dError = M_MAXERROR );
bool	UTIL_LessThanOrEqualError( double dTest1, double dTest2, double dError = M_MAXERROR );
bool	UTIL_GreaterThanOrEqualError( double dTest1, double dTest2, double dError = M_MAXERROR );
bool	UTIL_IsWithinError( double dTest, double dLow, double dHigh, double dError = M_MAXERROR );
bool	UTIL_IsWithinOrderlessError( double dTest, double dBound1, double dBound2, double dError = M_MAXERROR );
void	UTIL_RoundWithinError( double &dTest, double dCompare, double dError = M_MAXERROR );
void	UTIL_RoundWithinError( Vector &vecTest, const Vector &vecCompare, double dError = M_MAXERROR );

//================================================================================================================================================================
// Collision and such
//================================================================================================================================================================

bool UTIL_CheckCol( const Vector &vecPoint, const Vector &vecDisplacement, const Vector &vecLineP1, const Vector &vecLineP2, Vector *vecHitPoint = NULL );
bool UTIL_IsPointOnLine( const Vector &vecPoint, const Vector &vecLineP1, const Vector &vecLineP2 );
bool UTIL_LineCol( const Vector &vecLine1P1, const Vector &vecLine1P2, const Vector &vecLine2P1, const Vector &vecLine2P2, Vector *vecHitPoint = NULL );
bool UTIL_OneInfLineCol( const Vector &vecLine1P1, const Vector &vecLine1P2, const Vector &vecLine2P1, const Vector &vecLine2P2, Vector *vecHitPoint = NULL );
bool UTIL_InfLineCol( const Vector &vecLine1P1, const Vector &vecLine1P2, const Vector &vecLine2P1, const Vector &vecLine2P2, Vector *vecHitPoint = NULL );
void UTIL_ClosestPointToLine( const Vector &vecPoint, const Vector &vecLineP1, const Vector &vecLineP2, Vector *vecClosestPoint );
bool UTIL_CheckEdgeCases( const Vector &vecHitPoint, const Vector &vecLine1P1, const Vector &vecLine1P2, const Vector &vecLine2P1, const Vector &vecLine2P2 );
bool UTIL_LineThroughCircle( const Vector &vecOrigin, double dRadius, const Vector &vecLineP1, const Vector &vecLineP2 );

#if (0)
int UTIL_GetPointsOfLineOnCircle( const Vector &vecOrigin, double dRadiusSqr, const Vector &vecLineP1, const Vector &vecLineP2, Vector *vecColP1 = NULL, Vector *vecColP2 = NULL );
bool UTIL_GetAdjustedAngle( const Vector &vecOrigin, const Vector &vecEdgePoint, const Vector &vecLineP1, const Vector &vecLineP2, double &dAngle );
bool UTIL_GetAdjustedAngle( const Vector &vecOrigin, const Vector &vecEdgePoint, const Vector &vecTestPoint, double &dAngle );
bool UTIL_LineInAngDirection( double dAngle, const Vector &vecOrigin, const Vector &vecPoint, const Vector &vecNormal );
#endif

#endif // MATH_H