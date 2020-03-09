#pragma once



class vec3
{
public:
	vec3() {}
	vec3(float i, float j, float k)
	{
		x = i;
		y = j;
		z = k;
	}
	float x, y, z;

	void operator=(const vec3& other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
	}

	vec3 operator*(const float& other)
	{
		vec3 out;
		out.x = this->x * other;
		out.y = this->y * other;
		out.z = this->z * other;
		return out;
	}

	vec3 operator/(const float& other)
	{
		vec3 out;
		out.x = this->x / other;
		out.y = this->y / other;
		out.z = this->z / other;
		return out;
	}

	vec3 operator+(const vec3& other)
	{
		vec3 out;
		out.x = this->x + other.x;
		out.y = this->y + other.y;
		out.z = this->z + other.z;
		return out;
	}

	vec3 operator-(const vec3& other)
	{
		vec3 out;
		out.x = this->x - other.x;
		out.y = this->y - other.y;
		out.z = this->z - other.z;
		return out;
	}

	friend ostream& operator<<(ostream& os, const vec3& v)
	{
		os << v.x << ' ' << v.y << ' ' << v.z;
		return os;
	}
};

float dist(vec3 a, vec3 b)
{
	float A1 = (a.x - b.x);
	A1 *= A1;
	float A2 = (a.y - b.y);
	A2 *= A2;
	float A3 = (a.z - b.z);
	A3 *= A3;


	return sqrt(A1 + A2 + A3);
}

class Potential
{
public:
	Potential() {}
	vec3 pos;
	vec3 vel;
	vec3 acc;

	class th
	{
	public:
		th() {}
		vec3 pos;
		vec3 vel;
		vec3 acc;
	}prev;


};



int JGN_NPotential = 0;
Potential **JGN_RAMPotential = NULL;
float dt = 0.001;
float jgn_resist = 0.01;
vec3 gravity(0, 0, 0);




Potential* JGN_AddPotential(vec3 coord)
{
	Potential* pot = new Potential;

	//////////////////initialize position velocity acceleration
	pot->pos = coord;
	pot->vel = vec3(0, 0, 0);
	pot->prev.pos = coord;
	pot->prev.vel = vec3(0, 0, 0);
	pot->prev.acc = vec3(0, 0, 0);

	JGN_NPotential++;
	JGN_RAMPotential = (Potential**)realloc(JGN_RAMPotential, sizeof(Potential)*JGN_NPotential);
	JGN_RAMPotential[JGN_NPotential - 1] = pot;
	//cout << JGN_RAMPotential[0]; getchar();

	return pot;
}

void JGN_DestroyPotential(Potential* pot)
{
	for (int i = 0; i < JGN_NPotential; i++)
	{
		if (JGN_RAMPotential[i] == pot)
		{
			JGN_RAMPotential[i] = JGN_RAMPotential[JGN_NPotential - 1];
		}
	}
	delete pot;
	JGN_NPotential--;
	JGN_RAMPotential = (Potential**)realloc(JGN_RAMPotential, sizeof(Potential)*JGN_NPotential);
}


float GausianForce(float T, float r)
{
	if (r < 20 * T)//if it is within range calculate
	{
		float a = exp((0.3 - (r-0.3)) / T);
		float aa = (1 + a);
		float b = exp((0.3 + (r - 0.3)) / T);
		float bb = (1 + b);


		return a / ((aa*aa)*T) - b / ((bb*bb)*T);
	}
	else
	{
		return 0;
	}
}

float Power2Force(float T, float r)
{//T=5000 nice
		return T * (r - 0.5);
}

void JGN_UpdatePotentialPositions()
{
	float T = 5000;
	Potential **p = JGN_RAMPotential;

	
	//pre process
	for (int i = 0; i < JGN_NPotential; i++)
	{
		//prev position
		p[i]->prev.pos = p[i]->pos;

		//calc new position
		p[i]->pos = p[i]->pos + p[i]->vel*dt + p[i]->prev.acc*(dt*dt / 2.0);

		//zero all forces
		p[i]->acc = gravity;
	}





//#pragma omp parallel for //firstprivate(T, p)
	for (int i = 0; i < JGN_NPotential; i++)//for every atom
	{
		for (int j = 0; j < JGN_NPotential; j++)//scan every other atom
		{
			if (i != j)//if it's not the same atom
			{
				float R = dist(JGN_RAMPotential[i]->pos, JGN_RAMPotential[j]->pos);

				float a = Power2Force(T, R);

				if(a!=0)
				p[i]->acc = p[i]->acc + (p[j]->pos - p[i]->pos) * (a/ R);

				
			}

		}
	}

	for (int i = 0; i < JGN_NPotential; i++)
	{
		p[i]->vel = p[i]->vel + (p[i]->prev.acc + p[i]->acc)*(dt / 2.0);
		p[i]->vel = p[i]->vel * (1- jgn_resist);
		p[i]->prev.acc = p[i]->acc;
	}
}