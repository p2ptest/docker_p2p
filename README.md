# docker_p2p
p2p编译环境镜像

## 生成编译镜像
```
cd compile
//git不能存储空目录,但是编译时需要，所以自己创建
mkdir -p server/framecommon/lib server/framecommon/include/framecommon
mkdir -p server/srvframe/obj server/srvframe/lib server/srvframe/include
mkdir -p server/utils/include/json server/utils/include/hiredis server/utils/lib

docker build -t p2p/compile .
```
- Dockerfile会安装p2p编译的依赖库
- public和server目录是底层库，拷贝到镜像的/opt目录下，并编译

## 运行示例

```cd ../run_example```

### 编译

- --rm 退出后删除
- -v 配置数据卷
- -w 设置工作目录

##### 1. 编译CdnCtlMgrCenter
```
docker run --rm -v ${PWD}/cdn/CdnCtlMgrCenter:/opt/server/cdn/CdnCtlMgrCenter \
-w /opt/server/cdn/CdnCtlMgrCenter p2p/compile \
sh -c "chmod +x *.sh;./build.sh"
```

##### 2. 编译cdnmaster
```
PRO_DIR=cdn/cdnmaster
docker run --rm \
-v ${PWD}/${PRO_DIR}:/opt/server/${PRO_DIR} \
-w /opt/server/${PRO_DIR} p2p/compile \
sh -c "chmod +x *.sh;./build.sh"
```

### 生成运行镜像
```
docker build -t p2p/cdn .
```

### 运行
- -d 后台运行
- --name 起名字
- -p 映射端口

```
docker run --name CdnCtlMgr -p 23231:23231 -w /opt/server/cdn/CdnCtlMgrCenter p2p/cdn ./CdnCtlMgrCenter -l
```
