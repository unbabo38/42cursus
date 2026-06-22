#!/bin/bash
# 5秒間わざとスリープ（ブロッキングを発生させる）
sleep 10

echo "Status: 200 OK"
echo "Content-Type: text/html"
echo ""
echo "<html><body><h1>CGI Done!</h1></body></html>"
