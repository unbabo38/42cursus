
// master () {
// 	設定の読み取りと検証
// 	ソケットの作成、結合、閉じること
// 	設定されたプロセス数の開始、終了、維持worker
// 	サービス中断なしの再構成
// 	ノンストップのバイナリアップグレードの制御(新しいバイナリの開始と 必要なら巻き戻す)
// 	ログファイルの再開
// 	埋め込みPerlスクリプトのコンパイル
// }

// http {
// リクエスト処理を初期化します。
// プロセスヘッダー。
// 処理体。
// 関連するハンドラーに連絡してください。
// 処理段階を進めてください。
// }


// ngx_worker_process_cycle() {
// OS固有のメカニズム(例:または)を持つプロセスイベント。epollkqueue
// イベントを受け入れ、関連するアクションを実行してください。
// プロセス/プロキシリクエストのヘッダーと本文。
// レスポンスコンテンツ(ヘッダー、ボディ)を生成し、 クライアント。
// 要望を最終決定。
// タイマーとイベントを再初期化します。
// }

#include <sys/socket.h>

int main() {
//   master;
  int server_fd, new_socket; long valread;
  struct sockaddr_in address;
  int addrlen = sizeof(address);

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  loop(worker);
}
