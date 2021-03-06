#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ll long long

// alice and bob asets
typedef struct alice {
    int x;
} Alice;
typedef struct bob {
    int a;
    int b;
} Bob;


// public data
int p = 0; // modulus
int g = 0; // generator
int z = 0; // signature
int m = 0; // message
int y = 0; // public key


// users asets
Alice alice = {0,};
Bob bob = {0,};


int powmod(int p, int n, int m) { // pow and modular
    int result = 1;

    while(n--)
        result = (result*p)%m;
    
    return result;
}


int mul_inv(int a, int b) { // euclide algorithm
	int b0 = b, t, q;
	int x0 = 0, x1 = 1;
	if (b == 1) return 1;
	while (a > 1) {
		q = a / b;
		t = b, b = a % b, a = t;
		t = x0, x0 = x1 - q * x0, x1 = t;
	}
	if (x1 < 0) x1 += b0;
	return x1;
}


int is_prime(int n) { // prime is 1, no prime is 0
    if( n <= 1 )
        return 0;
         
    if( (n&1) == 0 )
        return (n == 2);
         
    for(int i=3; i*i<=n; i+=2) { 
        if( n % i == 0) {
            return 0;
        }
    }
    return 1;
}


int gcd(int a, int b) { // gcd check
	return b ? gcd(b, a%b) : a;
}


void key_setup() { // Alices setup key
    do {
        printf("input prime number p : "); scanf("%d", &p);
    } while(!is_prime(p));

    do {
        printf("input GCD(x, p-1) x : "); scanf("%d", &alice.x);
    } while(gcd(alice.x, p-1)!=1);
    
    printf("input g over GF(p) : "); scanf("%d", &g);

    y = powmod(g, alice.x, p); // make public key
}


int signing(int m) { // Alice's signing
    key_setup();
    printf("g : %d\n", g);
    printf("m : %d, x : %d\n", m, alice.x);
    z = powmod(m, alice.x, p);
    printf("y : %d\n", y);
    printf("z : %d\n", z);

    return z;
}


int bob_computes_c() { // Bob computes c
    int a, b; // random numbers
    int c; // chiper

    bob.a = rand()%p; // choose random a, b
    bob.b = rand()%p;

    printf("a: %d, b: %d\n", bob.a, bob.b);
    c = (powmod(z, bob.a, p)*powmod(y, bob.b, p)%p); // computes c

    return c;
}


int alice_computes_d(int c) { // Alice computes d whith c
    int d; // decode
    int xi; // inverse x

    xi = mul_inv(alice.x, p-1); // get inverse x
    printf("xi : %d\n", xi);
    d = powmod(c, xi, p); // computes d

    return d;
}


int bob_confirms(int d_a) { // Bob confirms d computed by Alice and Bob
    int d_b; // made by bob

    d_b = (powmod(m, bob.a, p)*powmod(g, bob.b, p))%p; // computes d
    printf("Bob computed d : %d\n", d_b);

    return d_a==d_b?1:0;
}


int verifying() { // verifying process
    int c; // chiper
    int d; // decode

    c = bob_computes_c(); // Bob made c
    printf("c : %d\n", c);
    d = alice_computes_d(c); // Alice made d from c
    printf("Alice return d : %d\n", d);
    if(bob_confirms(d)) { // Bob confirms Alice's d and Bob's d
        printf("This signature is true!\n");
    }
    else {
        printf("This signature is false!\n");
    }
}


int main(int argc, char* argv[]) {
    if(argc < 2) {
        printf("Need more message(integer)!\n");
        return 1;
    }

    srand(time(NULL)); // random seed is timestamp

    m = atoi(argv[1]); // message

    signing(m); // signing
    verifying(); // verifying

    return 0;
}
