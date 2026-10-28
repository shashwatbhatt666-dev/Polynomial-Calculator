#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Term {
    int coeff;
    int exp;
    struct Term *next;
} Term;

Term *createTerm(int coeff, int exp) {
    Term *t = malloc(sizeof(Term));
    if (!t) { perror("malloc"); exit(1); }
    t->coeff = coeff;
    t->exp = exp;
    t->next = NULL;
    return t;
}


void insertTerm(Term **head, int coeff, int exp) {
    if (coeff == 0) return;
    Term *prev = NULL, *cur = *head;
    while (cur && cur->exp > exp) {
        prev = cur;
        cur = cur->next;
    }
    if (cur && cur->exp == exp) {
        cur->coeff += coeff;
        if (cur->coeff == 0) {
            if (prev) prev->next = cur->next;
            else *head = cur->next;
            free(cur);
        }
        return;
    }
    Term *t = createTerm(coeff, exp);
    t->next = cur;
    if (prev) prev->next = t;
    else *head = t;
}

void display(Term *p) {
    if (!p) { printf("0\n"); return; }
    for (Term *t = p; t; t = t->next) {
        int c = t->coeff;
        if (t != p) {
            printf(c < 0 ? " - " : " + ");
            c = abs(c);
        } else if (c < 0) {
            printf("-");
            c = -c;
        }
        if (t->exp == 0) {
            printf("%d", c);
        } else {
            if (c != 1) printf("%d", c);
            printf("x");
            if (t->exp != 1) printf("^%d", t->exp);
        }
    }
    printf("\n");
}

Term *addPoly(Term *a, Term *b) {
    Term *r = NULL;
    for (Term *t = a; t; t = t->next) insertTerm(&r, t->coeff, t->exp);
    for (Term *t = b; t; t = t->next) insertTerm(&r, t->coeff, t->exp);
    return r;
}

Term *subPoly(Term *a, Term *b) {
    Term *r = NULL;
    for (Term *t = a; t; t = t->next) insertTerm(&r, t->coeff, t->exp);
    for (Term *t = b; t; t = t->next) insertTerm(&r, -t->coeff, t->exp);
    return r;
}

Term *mulPoly(Term *a, Term *b) {
    Term *r = NULL;
    for (Term *i = a; i; i = i->next)
        for (Term *j = b; j; j = j->next)
            insertTerm(&r, i->coeff * j->coeff, i->exp + j->exp);
    return r;
}

double evaluate(Term *p, double x) {
    double sum = 0;
    for (Term *t = p; t; t = t->next)
        sum += t->coeff * pow(x, t->exp);
    return sum;
}

void freePoly(Term *p) {
    while (p) {
        Term *next = p->next;
        free(p);
        p = next;
    }
}

Term *readPoly(const char *name) {
    Term *p = NULL;
    int n, c, e;
    printf("Number of terms in %s: ", name);
    if (scanf("%d", &n) != 1 || n < 0) { while (getchar() != '\n'); return NULL; }
    for (int i = 0; i < n; i++) {
        printf("  term %d (coeff exp): ", i + 1);
        if (scanf("%d %d", &c, &e) != 2 || e < 0) {
            printf("  invalid input, skipping\n");
            while (getchar() != '\n');
            continue;
        }
        insertTerm(&p, c, e);
    }
    return p;
}

int main(void) {
    int choice;
    do {
        printf("\n--- Polynomial Calculator ---\n");
        printf("1. Add\n2. Subtract\n3. Multiply\n4. Evaluate\n0. Exit\nChoice: ");
        if (scanf("%d", &choice) != 1) break;

        if (choice >= 1 && choice <= 3) {
            Term *a = readPoly("A");
            Term *b = readPoly("B");
            Term *r = NULL;
            if (choice == 1) r = addPoly(a, b);
            else if (choice == 2) r = subPoly(a, b);
            else r = mulPoly(a, b);
            printf("A = "); display(a);
            printf("B = "); display(b);
            printf("Result = "); display(r);
            freePoly(a); freePoly(b); freePoly(r);
        } else if (choice == 4) {
            Term *p = readPoly("P");
            double x;
            printf("Value of x: ");
            scanf("%lf", &x);
            printf("P(x) = "); display(p);
            printf("P(%g) = %g\n", x, evaluate(p, x));
            freePoly(p);
        }
    } while (choice != 0);
    return 0;
}
