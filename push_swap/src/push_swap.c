#include <stdio.h>
#include <stdlib.h>

typedef struct s_node {
    int value;
    struct s_node *next;
} t_node;

// --- ユーティリティ関数群 ---

// リストを配列にコピー
void list_to_array(t_node *head, int *arr, int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = head->value;
        head = head->next;
    }
}

// a に b のトップ値を挿入するのに必要な ra 回数を計算
// arr: a を list_to_array でコピーした配列
int find_insert_idx(int *arr, int n, int v) {
    int min = arr[0], max = arr[0], idx_min = 0;
    for (int i = 1; i < n; i++) {
        if (arr[i] < min) { min = arr[i]; idx_min = i; }
        if (arr[i] > max) max = arr[i];
    }
    // (1) v が最小値未満または最大値超過なら「最小値の直前」に入れる
    if (v < min || v > max) return idx_min;
    // (2) 通常の昇順区間を探す
    for (int i = 0; i < n; i++) {
        int nxt = (i + 1) % n;
        if (arr[i] < v && v < arr[nxt]) return nxt;
    }
    return 0;
}

// 先頭にノードを push
void push_front(t_node **head, int v) {
    t_node *n = malloc(sizeof(*n));
    n->value = v;
    n->next = *head;
    *head = n;
}
// 末尾にノードを push
void push_back(t_node **head, int v) {
    t_node *n = malloc(sizeof(*n));
    n->value = v;
    n->next = NULL;
    if (!*head) { *head = n; return; }
    t_node *t = *head;
    while (t->next) t = t->next;
    t->next = n;
}
// 先頭ノードを pop してその値を返す
int pop_front(t_node **head) {
    t_node *t = *head;
    int v = t->value;
    *head = t->next;
    free(t);
    return v;
}
// リストの長さを数える
int count_nodes(t_node *head) {
    int c = 0;
    while (head) { c++; head = head->next; }
    return c;
}

// ra: 先頭を末尾へ
void ra(t_node **head) {
    if (!*head || !(*head)->next) return;
    int v = pop_front(head);
    push_back(head, v);
    printf("ra\n");
}
// rra: 末尾を先頭へ
void rra(t_node **head) {
    if (!*head || !(*head)->next) return;
    t_node *prev = NULL, *t = *head;
    while (t->next) {
        prev = t;
        t = t->next;
    }
    prev->next = NULL;
    push_front(head, t->value);
    free(t);
    printf("rra\n");
}
// pa: b の先頭を a の先頭に
void pa(t_node **a, t_node **b) {
    if (!*b) return;
    int v = pop_front(b);
    push_front(a, v);
    printf("pa\n");
}

// スタック内容を出力
void print_stack(t_node *s, const char *name) {
    printf("%s: ", name);
    while (s) { printf("%d ", s->value); s = s->next; }
    printf("\n");
}

// --- メイン ---
int main() {
    // 初期化：a は昇順、b はランダム
    int avals[] = {2, 4, 7, 9};
    t_node *a = NULL;
    for (int i = 3; i >= 0; i--) push_front(&a, avals[i]);
    int bvals[] = {8, 3, 5};
    t_node *b = NULL;
    for (int i = 2; i >= 0; i--) push_front(&b, bvals[i]);

    printf("Before:\n");
    print_stack(a, "a");
    print_stack(b, "b");

    // --- ターク法本体：b → a に一つずつ戻す ---
    while (b) {
        int n = count_nodes(a);
        int *arr = malloc(sizeof(int) * n);
        list_to_array(a, arr, n);

        int v = b->value;
        int ri = find_insert_idx(arr, n, v);
        free(arr);

        // 回転の最短手順で先頭に「挿入位置」を合わせる
        if (ri <= n/2) {
            for (int i = 0; i < ri; i++) ra(&a);
        } else {
            for (int i = 0; i < n - ri; i++) rra(&a);
        }
        pa(&a, &b);
    }

    // --- 最後に、最小値を先頭に持ってくる ---
    {
        int n = count_nodes(a);
        int *arr = malloc(sizeof(int) * n);
        list_to_array(a, arr, n);
        // 最小値のインデックスを探す
        int idx_min = 0;
        for (int i = 1; i < n; i++) {
            if (arr[i] < arr[idx_min]) idx_min = i;
        }
        free(arr);
        // 先頭までの回転
        if (idx_min <= n/2) {
            for (int i = 0; i < idx_min; i++) ra(&a);
        } else {
            for (int i = 0; i < n - idx_min; i++) rra(&a);
        }
    }

    printf("After:\n");
    print_stack(a, "a");
    print_stack(b, "b");
    return 0;
}
