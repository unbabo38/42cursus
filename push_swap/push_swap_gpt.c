#include "./include/push_swap_gpt.h"
#include <stdlib.h>

// ===== Node utilities =====
// Create a new linked-list node
static t_list *new_node(int value) {
    t_list *node = malloc(sizeof(t_list));
    if (!node) exit(1);
    node->value = value;
    node->next = NULL;
    return node;
}

// Append a node to the end of the list
static void add_back(t_list **head, t_list *new) {
    if (!*head) {
        *head = new;
        return;
    }
    t_list *tmp = *head;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new;
}

// ===== Parsing and basic checks =====
// Parse command-line arguments into stack A
static t_list *parse_args(int argc, char **argv) {
    t_list *a = NULL;
    for (int i = 1; i < argc; i++) {
        int val = atoi(argv[i]);
        add_back(&a, new_node(val));
    }
    return a;
}

// Check if stack A is already sorted (ascending)
static int is_sorted(t_list *a) {
    while (a && a->next) {
        if (a->value > a->next->value)
            return 0;
        a = a->next;
    }
    return 1;
}

// Count number of nodes in a list
static int count_lists(t_list *lst) {
    int cnt = 0;
    while (lst) { cnt++; lst = lst->next; }
    return cnt;
}

// ===== Rank compression =====
// Compress values to 0..N-1 ranks stored in node->rank
static void compress_ranks(t_list **a) {
    int n = count_lists(*a);
    int *arr = malloc(sizeof(int) * n);
    t_list *p = *a;
    for (int i = 0; p; i++, p = p->next)
        arr[i] = p->value;
    // sort arr
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (arr[i] > arr[j]) { int t = arr[i]; arr[i] = arr[j]; arr[j] = t; }
    // assign ranks
    p = *a;
    while (p) {
        for (int i = 0; i < n; i++) {
            if (arr[i] == p->value) {
                p->rank = i;
                break;
            }
        }
        p = p->next;
    }
    free(arr);
}

// ===== LIS marking =====
static int *find_lis_indexes(t_list *a, int *lis_len) {
    int size = count_lists(a);
    int *vals = malloc(sizeof(int) * size);
    int *len = malloc(sizeof(int) * size);
    int *prev = malloc(sizeof(int) * size);
    t_list *t = a;
    for (int i = 0; t; i++, t = t->next) vals[i] = t->value;
    for (int i = 0; i < size; i++) { len[i] = 1; prev[i] = -1; }
    int maxl = 1, end = 0;
    for (int i = 1; i < size; i++) {
        for (int j = 0; j < i; j++) {
            if (vals[j] < vals[i] && len[j] + 1 > len[i]) {
                len[i] = len[j] + 1;
                prev[i] = j;
            }
        }
        if (len[i] > maxl) { maxl = len[i]; end = i; }
    }
    *lis_len = maxl;
    int *indexes = malloc(sizeof(int) * maxl);
    for (int i = maxl - 1, k = end; i >= 0; i--) {
        indexes[i] = k;
        k = prev[k];
    }
    free(vals); free(len); free(prev);
    return indexes;
}

// Mark LIS nodes: keep=1 for LIS, keep=0 otherwise
static void mark_lis_nodes(t_list *a) {
    int lis_len;
    int *idxs = find_lis_indexes(a, &lis_len);
    t_list *t = a;
    for (int i = 0; t; i++, t = t->next) {
        t->keep = 0;
        for (int j = 0; j < lis_len; j++)
            if (i == idxs[j]) t->keep = 1;
    }
    free(idxs);
}

// ===== Chunk push =====
static int min(int x, int y) { return x < y ? x : y; }
static void push_non_lis_in_chunks(t_list **a, t_list **b, int K) {
    int total = count_lists(*a);
    int lis_count = 0;
    for (t_list *t = *a; t; t = t->next) if (t->keep) lis_count++;
    int non = total - lis_count;
    int chunk = (non + K - 1) / K;
    for (int i = 0; i < K; i++) {
        int lower = i * chunk;
        int upper = lower + chunk - 1;
        int pushed = 0;
        int need = min(chunk, non - lower);
        while (pushed < need) {
            if ((*a)->keep == 0 && (*a)->rank >= lower && (*a)->rank <= upper) {
                pb(a, b);
                pushed++;
                if ((*b)->rank > lower + chunk/2)
                    rb(b);
            } else {
                ra(a);
            }
        }
    }
}

// ===== B→A insertion =====
static int find_insert_position_in_a(t_list *a, int val) {
    int pos = 0;
    int minv = a->value, maxv = a->value;
    for (t_list *t = a; t; t = t->next) {
        if (t->value < minv) minv = t->value;
        if (t->value > maxv) maxv = t->value;
    }
    t_list *t = a;
    if (val < minv || val > maxv) {
        while (t->value != minv) { t = t->next; pos++; }
        return pos;
    }
    pos = 0; t = a;
    while (t->next) {
        if (t->value < val && val < t->next->value)
            return pos + 1;
        t = t->next; pos++;
    }
    return 0;
}

static t_cheap calculate_b_to_a(t_list *b, t_list *a) {
    t_list *t = b;
    t_cheap best = { .cost = INT_MAX };
    int size_b = count_lists(b);
    int size_a = count_lists(a);
    int idx = 0;
    while (t) {
        int pa = find_insert_position_in_a(a, t->value);
        int ra = pa;
        int rra = size_a - ra;
        int rb = idx;
        int rrb = size_b - rb;
        int c1 = max(ra, rb);
        int c2 = max(rra, rrb);
        int c3 = ra + rrb;
        int c4 = rra + rb;
        int cost = c1;
        if (c2 < cost) cost = c2;
        if (c3 < cost) cost = c3;
        if (c4 < cost) cost = c4;
        int pat = (cost == c1 ? 1 : cost == c2 ? 2 : cost == c3 ? 3 : 4);
        if (cost < best.cost) {
            best.cost = cost;
            best.ra = ra;
            best.rb = rb;
            best.pat = pat;
            best.node = t;
        }
        if (cost == 1) break;
        t = t->next;
        idx++;
    }
    return best;
}

static void execute_move_b_to_a(t_list **b, t_list **a, t_cheap mv) {
    int nb = count_lists(*b);
    int na = count_lists(*a);
    int rrb = nb - mv.rb;
    int rra = na - mv.ra;
    int m;
    switch (mv.pat) {
    case 1:
        m = min(mv.ra, mv.rb);
        for (int i = 0; i < m; i++) rr(a, b);
        for (int i = m; i < mv.ra; i++) ra(a);
        for (int i = m; i < mv.rb; i++) rb(b);
        break;
    case 2:
        m = min(rra, rrb);
        for (int i = 0; i < m; i++) rrr(a, b);
        for (int i = m; i < rra; i++) rra(a);
        for (int i = m; i < rrb; i++) rrb(b);
        break;
    case 3:
        for (int i = 0; i < mv.ra; i++) ra(a);
        for (int i = 0; i < rrb; i++) rrb(b);
        break;
    case 4:
        for (int i = 0; i < rra; i++) rra(a);
        for (int i = 0; i < mv.rb; i++) rb(b);
        break;
    }
    if (*b)
        pa(b, a);
}

// ===== Final rotate =====
static void rotate_a_to_min(t_list **a) {
    int minv = (*a)->value;
    int pos = 0;
    for (t_list *t = *a; t; t = t->next) {
        if (t->value < minv) minv = t->value;
    }
    t_list *t = *a;
    while (t->value != minv) { pos++; t = t->next; }
    int n = count_lists(*a);
    if (pos <= n/2) {
        while ((*a)->value != minv) ra(a);
    } else {
        while ((*a)->value != minv) rra(a);
    }
}

// ===== main =====
int main(int argc, char **argv) {
    t_list *a = parse_args(argc, argv);
    t_list *b = NULL;
    if (!a || is_sorted(a)) return 0;
    compress_ranks(&a);
    mark_lis_nodes(a);
    push_non_lis_in_chunks(&a, &b, 10);
    while (count_lists(b)) {
        t_cheap mv = calculate_b_to_a(b, a);
        execute_move_b_to_a(&b, &a, mv);
    }
    rotate_a_to_min(&a);
    return 0;
}
