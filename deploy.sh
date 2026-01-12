#!/bin/bash
set -e

echo "Step 1: 正在清理并重新编译..."
# 确保在当前目录编译，这样 index.js 里的路径是 ./index.data
emcc src/*.cpp -o index.html \
  -s USE_SDL=2 \
  -s USE_SDL_IMAGE=2 \
  -s USE_SDL_TTF=2 \
  -s USE_SDL_MIXER=2 \
  -s SDL2_IMAGE_FORMATS='["png"]' \
  -s SDL2_MIXER_FORMATS='["mp3","ogg"]' \
  --preload-file assets@assets \
  -O2

echo "Step 2: 安全清理 out-branch 内容..."
# 检查挂载点是否正常
if [ ! -f "out-branch/.git" ]; then
    echo "错误：out-branch/.git 丢失，尝试恢复..."
    git worktree prune && git worktree add out-branch gh-pages
fi

# 使用 find 只删除非隐藏文件和非隐藏文件夹
# 这样可以确保 .git 文件永远不会被删掉
find out-branch/* -maxdepth 0 -not -name ".git" -exec rm -rf {} +

echo "Step 3: 将新文件移入并部署..."
# 将编译好的 4 个文件移入
cp index.html index.js index.wasm index.data out-branch/

# 进入目录进行 Git 操作
cd out-branch
# 重点：强制添加所有文件并刷新索引
git add -A


# 2. 强制提交 (即使 Git 认为内容没变，也会产生一个新的 Commit)
# --allow-empty 允许产生一个内容没变化但 ID 不同的提交
git commit --allow-empty -m "Manual Re-deploy: $(date +'%Y-%m-%d %H:%M:%S')"

# 3. 推送
git push origin gh-pages

cd ..

# 清理当前目录的临时编译产物（可选）
rm index.html index.js index.wasm index.data