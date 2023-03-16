echo "启动升级包：$1";
if [ -n "$1" ];then
    exec $1 &
fi

