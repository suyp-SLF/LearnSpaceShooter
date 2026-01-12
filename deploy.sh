#!/bin/bash

# 1. 确保在主分支编译出最新的文件
echo "正在编译最新版本..."
mkdir -p dist
emcc src/*.cpp -o dist/index.html \
  -s USE_SDL=2 \
  -s USE_SDL_IMAGE=2 \
  -s USE_SDL_TTF=2 \
  -s USE_SDL_MIXER=2 \
  -s SDL2_IMAGE_FORMATS='["png"]' \
  -s SDL2_MIXER_FORMATS='["mp3","ogg"]' \
  --preload-file assets@assets \
  -O2

# 2. 同步到 out-branch (也就是你的 gh-pages 分支挂载点)
echo "正在同步到发布分支..."
# 清理旧文件（保留 .git 目录）
find out-branch/ -maxdepth 1 ! -name '.git' ! -name 'out-branch/' -exec rm -rf {} +
# 拷贝新生成的编译结果
cp -r dist/* out-branch/

# 3. 提交并推送发布分支
echo "正在推送更新到远程..."
cd out-branch
git add .
git commit -m "Update game build: $(date +'%Y-%m-%d %H:%M:%S')"
git push origin gh-pages

# 4. 返回主目录提交源码（可选，但建议）
# cd ..
# echo "发布完成！别忘了提交你的 C++ 源码更新："
# git add .
# git commit -m "Source code changes for build $(date +'%Y-%m-%d')"
# git push origin main