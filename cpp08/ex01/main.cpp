#include "Span.hpp"
#include <iostream>
#include <list>
int main() {
  std::cout << "--- 🟢 正常系のテスト ---" << std::endl;
  try {
    Span s(3);
    s.addNumber(6);
    s.addNumber(3);
    s.addNumber(17);

    // 💡 std::cout で囲むことで、計算結果を画面に表示する！
    std::cout << "Shortest Span: " << s.shortestSpan() << " (期待値: 3)"
              << std::endl;
    std::cout << "Longest Span : " << s.longestSpan() << " (期待値: 14)"
              << std::endl;

  } catch (const std::exception &e) {
    std::cout << "例外が発生しました: " << e.what() << std::endl;
  }

  std::cout << "\n--- 🔴 異常系のテスト（要素が足りない場合） ---" << std::endl;
  try {
    Span s2(5);
    s2.addNumber(42);

    // 🚨 要素が1個しかないので、ここで logic_error が発生するはず！
    std::cout << s2.shortestSpan() << std::endl;
  } catch (const std::exception &e) {
    std::cout << "🎯 期待通りエラーをキャッチ: " << e.what() << std::endl;
  }

  try {
    Span sp(10000);
    std::vector<int> sample_vector;

    // vector に 0 から 9999 までの数字を詰める
    for (int i = 0; i < 10000; i++) {
      sample_vector.push_back(i);
    }

    // 💡 自作の addNumbers で一気に流し込む！
    sp.addNumbers(sample_vector.begin(), sample_vector.end());

    std::cout << "流し込み成功！" << std::endl;
    std::cout << "1万個の Shortest Span: " << sp.shortestSpan()
              << " (期待値: 1)" << std::endl;
    std::cout << "1万個の Longest Span : " << sp.longestSpan()
              << " (期待値: 9999)" << std::endl;

  } catch (const std::exception &e) {
    std::cout << "❌ エラーが発生しました: " << e.what() << std::endl;
  }

  // ────────────────────────────────────────────────────────
  std::cout << "\n--- 🔗 2. list (別のコンテナ) から一気に追加テスト ---"
            << std::endl;
  // ────────────────────────────────────────────────────────
  try {
    Span sp_list(5);
    std::list<int> sample_list;

    sample_list.push_back(10);
    sample_list.push_back(100);
    sample_list.push_back(50);

    // 💡 コンテナが std::list に変わっても、全く同じように流し込める！
    sp_list.addNumbers(sample_list.begin(), sample_list.end());

    std::cout << "流し込み成功！" << std::endl;
    std::cout << "List版 Shortest Span: " << sp_list.shortestSpan()
              << " (期待値: 40)" << std::endl;
    std::cout << "List版 Longest Span : " << sp_list.longestSpan()
              << " (期待値: 90)" << std::endl;

  } catch (const std::exception &e) {
    std::cout << "❌ エラーが発生しました: " << e.what() << std::endl;
  }

  return 0;
}
