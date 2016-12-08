#!/bin/bash
function replace_armeabi_so(){
#armeabi
#replace ikey,hmac,parameter and appkey into core.so
cd $OPAPKPATH/
rm -rf $OPAPKPATH/tmp/armeabi/*
cp $OPAPKPATH/armeabi/* $OPAPKPATH/tmp/armeabi/
echo -e \\n
echo "armeabi-->替换libcmcc_noah.so中的ikey，hmac和appkey,dec_key,是否派生标记和parameter值"
$OPAPKPATH/bin/soupdater $OPAPKPATH/tmp/armeabi/libcmcc_noah.so 4 5
#encrypt core.so
cd $OPAPKPATH/bin/
$OPAPKPATH/bin/encrypt $OPAPKPATH/tmp/armeabi/libcmcc_noah.so 6 $OPAPKPATH/tmp/armeabi/cmcc_noah
mv $OPAPKPATH/bin/ikey.reinforce $OPAPKPATH/bin/ikey_d.reinforce
#calculate cmcc_noah hmac
$OPAPKPATH/bin/hmacmaker $OPAPKPATH/tmp/armeabi/cmcc_noah
mv $OPAPKPATH/bin/ikey.reinforce $OPAPKPATH/bin/ikey1.reinforce
mv $OPAPKPATH/bin/hmac.reinforce $OPAPKPATH/bin/hmac1.reinforce
echo "armeabi-->替换libcmcc_haze.so中的ikey,hmac,解密key和解密参数"
$OPAPKPATH/bin/soupdater $OPAPKPATH/tmp/armeabi/libcmcc_haze.so 6
rm *.reinforce
echo "计算libcmcc_haze.so的hmac"
$OPAPKPATH/bin/hmacmaker $OPAPKPATH/tmp/armeabi/libcmcc_haze.so
cat hmac.reinforce >> $OPAPKPATH/tmp/armeabi/cmcc_noah
cat ikey.reinforce >> $OPAPKPATH/tmp/armeabi/cmcc_noah
rm *.reinforce
echo "把cmcc_noah拼接到libcmcc_haze.so之后"
cat $OPAPKPATH/tmp/armeabi/cmcc_noah >> $OPAPKPATH/tmp/armeabi/libcmcc_haze.so
#获取libcmcc_noah.so的大小
echo -e \\n
echo "把libcmcc_noah.so的大小拼接在cmcc_noah之后"
$OPAPKPATH/bin/Add_L_after_core $OPAPKPATH/tmp/armeabi/libcmcc_noah.so $OPAPKPATH/tmp/armeabi/libcmcc_haze.so
cp $OPAPKPATH/tmp/armeabi/libcmcc_haze.so $OPAPKPATH/app/lib/armeabi/libcmcc_haze.so
}
function replace_armeabi_v7a_so(){
#armeabi-v7a
#replace ikey,hmac,parameter and appkey into core.so
cd $OPAPKPATH/
rm -rf $OPAPKPATH/tmp/armeabi-v7a/*
cp $OPAPKPATH/armeabi-v7a/* $OPAPKPATH/tmp/armeabi-v7a/
echo -e \\n
echo "armeabi-v7a-->替换libcmcc_noah.so中的ikey，hmac和appkey,dec_key,是否派生标记和parameter值"
$OPAPKPATH/bin/soupdater $OPAPKPATH/tmp/armeabi-v7a/libcmcc_noah.so 4 5
#encrypt core.so
cd $OPAPKPATH/bin/
$OPAPKPATH/bin/encrypt $OPAPKPATH/tmp/armeabi-v7a/libcmcc_noah.so 6 $OPAPKPATH/tmp/armeabi-v7a/cmcc_noah
mv $OPAPKPATH/bin/ikey.reinforce $OPAPKPATH/bin/ikey_d.reinforce
#calculate cmcc_noah hmac
$OPAPKPATH/bin/hmacmaker $OPAPKPATH/tmp/armeabi-v7a/cmcc_noah
mv $OPAPKPATH/bin/ikey.reinforce $OPAPKPATH/bin/ikey1.reinforce
mv $OPAPKPATH/bin/hmac.reinforce $OPAPKPATH/bin/hmac1.reinforce
echo "armeabi-v7a-->替换libcmcc_haze.so中的ikey,hmac,解密key和解密参数"
$OPAPKPATH/bin/soupdater $OPAPKPATH/tmp/armeabi-v7a/libcmcc_haze.so 6
rm *.reinforce
echo "计算libcmcc_haze.so的hmac"
$OPAPKPATH/bin/hmacmaker $OPAPKPATH/tmp/armeabi-v7a/libcmcc_haze.so
cat hmac.reinforce >> $OPAPKPATH/tmp/armeabi-v7a/cmcc_noah
cat ikey.reinforce >> $OPAPKPATH/tmp/armeabi-v7a/cmcc_noah
rm *.reinforce
echo "把cmcc_noah拼接到libcmcc_haze.so之后"
cat $OPAPKPATH/tmp/armeabi-v7a/cmcc_noah >> $OPAPKPATH/tmp/armeabi-v7a/libcmcc_haze.so
#获取libcmcc_noah.so的大小
echo -e \\n
echo "把libcmcc_noah.so的大小拼接在cmcc_noah之后"
$OPAPKPATH/bin/Add_L_after_core $OPAPKPATH/tmp/armeabi-v7a/libcmcc_noah.so $OPAPKPATH/tmp/armeabi-v7a/libcmcc_haze.so
cp $OPAPKPATH/tmp/armeabi-v7a/libcmcc_haze.so $OPAPKPATH/app/lib/armeabi-v7a/libcmcc_haze.so
}
function replace_armeabi_x86_so(){
#x86
#replace ikey,hmac,parameter and appkey into core.so
cd $OPAPKPATH/
rm -rf $OPAPKPATH/tmp/x86/*
cp $OPAPKPATH/x86/* $OPAPKPATH/tmp/x86/
echo -e \\n
echo "x86-->替换libcmcc_noah.so中的ikey，hmac和appkey,dec_key,是否派生标记和parameter值"
$OPAPKPATH/bin/soupdater $OPAPKPATH/tmp/x86/libcmcc_noah.so 4 5
#encrypt core.so
cd $OPAPKPATH/bin/
$OPAPKPATH/bin/encrypt $OPAPKPATH/tmp/x86/libcmcc_noah.so 6 $OPAPKPATH/tmp/x86/cmcc_noah
mv $OPAPKPATH/bin/ikey.reinforce $OPAPKPATH/bin/ikey_d.reinforce
#calculate cmcc_noah hmac
$OPAPKPATH/bin/hmacmaker $OPAPKPATH/tmp/x86/cmcc_noah
mv $OPAPKPATH/bin/ikey.reinforce $OPAPKPATH/bin/ikey1.reinforce
mv $OPAPKPATH/bin/hmac.reinforce $OPAPKPATH/bin/hmac1.reinforce
echo "x86-->替换libcmcc_haze.so中的ikey,hmac,解密key和解密参数"
$OPAPKPATH/bin/soupdater $OPAPKPATH/tmp/x86/libcmcc_haze.so 6
rm *.reinforce
echo "计算libcmcc_haze.so的hmac"
$OPAPKPATH/bin/hmacmaker $OPAPKPATH/tmp/x86/libcmcc_haze.so
cat hmac.reinforce >> $OPAPKPATH/tmp/x86/cmcc_noah
cat ikey.reinforce >> $OPAPKPATH/tmp/x86/cmcc_noah
rm *.reinforce
echo "把cmcc_noah拼接到libcmcc_haze.so之后"
cat $OPAPKPATH/tmp/x86/cmcc_noah >> $OPAPKPATH/tmp/x86/libcmcc_haze.so
#获取libcmcc_noah.so的大小
echo -e \\n
echo "把libcmcc_noah.so的大小拼接在cmcc_noah之后"
$OPAPKPATH/bin/Add_L_after_core $OPAPKPATH/tmp/x86/libcmcc_noah.so $OPAPKPATH/tmp/x86/libcmcc_haze.so
cp $OPAPKPATH/tmp/x86/libcmcc_haze.so $OPAPKPATH/app/lib/x86/libcmcc_haze.so
}
function replace_armeabi_mips_so(){
#mips
#replace ikey,hmac,parameter and appkey into core.so
cd $OPAPKPATH/
rm -rf $OPAPKPATH/tmp/mips/*
cp $OPAPKPATH/mips/* $OPAPKPATH/tmp/mips/
echo -e \\n
echo "mips-->替换libcmcc_noah.so中的ikey，hmac和appkey,dec_key,是否派生标记和parameter值"
$OPAPKPATH/bin/soupdater $OPAPKPATH/tmp/mips/libcmcc_noah.so 4 5
#encrypt core.so
cd $OPAPKPATH/bin/
$OPAPKPATH/bin/encrypt $OPAPKPATH/tmp/mips/libcmcc_noah.so 6 $OPAPKPATH/tmp/mips/cmcc_noah
mv $OPAPKPATH/bin/ikey.reinforce $OPAPKPATH/bin/ikey_d.reinforce
#calculate cmcc_noah hmac
$OPAPKPATH/bin/hmacmaker $OPAPKPATH/tmp/mips/cmcc_noah
mv $OPAPKPATH/bin/ikey.reinforce $OPAPKPATH/bin/ikey1.reinforce
mv $OPAPKPATH/bin/hmac.reinforce $OPAPKPATH/bin/hmac1.reinforce
echo "mips-->替换libcmcc_haze.so中的ikey,hmac,解密key和解密参数"
$OPAPKPATH/bin/soupdater $OPAPKPATH/tmp/mips/libcmcc_haze.so 6
rm *.reinforce
echo "计算libcmcc_haze.so的hmac"
$OPAPKPATH/bin/hmacmaker $OPAPKPATH/tmp/mips/libcmcc_haze.so
cat hmac.reinforce >> $OPAPKPATH/tmp/mips/cmcc_noah
cat ikey.reinforce >> $OPAPKPATH/tmp/mips/cmcc_noah
rm *.reinforce
echo "把cmcc_noah拼接到libcmcc_haze.so之后"
cat $OPAPKPATH/tmp/mips/cmcc_noah >> $OPAPKPATH/tmp/mips/libcmcc_haze.so
#获取libcmcc_noah.so的大小
echo -e \\n
echo "把libcmcc_noah.so的大小拼接在cmcc_noah之后"
$OPAPKPATH/bin/Add_L_after_core $OPAPKPATH/tmp/mips/libcmcc_noah.so $OPAPKPATH/tmp/mips/libcmcc_haze.so
cp $OPAPKPATH/tmp/mips/libcmcc_haze.so $OPAPKPATH/app/lib/mips/libcmcc_haze.so
}
function usage(){
echo "用法: ./start.sh \$1(source.apk) \$2(out.apk) \$3(01234567890123456789012345678901) \$4(path_of_the_project(no /or\ in the end))"
echo -e \\n
}

OPAPKPATH=$(pwd)
echo $OPAPKPATH
if [ "$#" = 0 ];then
	usage
else
if [ "$#" = 4 ];then
	echo $4
	OPAPKPATH="$4"
fi
echo -e \\n

echo 第一步："开始解包"
cd $OPAPKPATH/
java -jar $OPAPKPATH/tools/apktool.jar d -s $1 -o $OPAPKPATH/app
echo -e \\n

echo 第二步："建立so库目录"
if test -d "$OPAPKPATH/app/lib/armeabi" ; then
echo armeabi exist!
else
mkdir -p $OPAPKPATH/app/lib/armeabi
fi
#cp $OPAPKPATH/libss_dvm.so $OPAPKPATH/app/lib/armeabi/libss_dvm.so
#cp $OPAPKPATH/libss_dvm.so $OPAPKPATH/app/lib/armeabi-v7a/libss_dvm.so
#mkdir -p $OPAPKPATH/app/lib/x86
#mkdir -p $OPAPKPATH/app/lib/mips
mkdir -p $OPAPKPATH/app/assets
echo -e \\n

echo 第三步："开始压缩classes.dex"
mv $OPAPKPATH/app/classes.dex $OPAPKPATH/tmp/
cd  $OPAPKPATH/
mv $OPAPKPATH/tmp/classes.dex $OPAPKPATH/cmcc_march.zip
echo -e \\n

echo 第四步："开始计算classes.dex的hmac"
$OPAPKPATH/bin/hmacmaker $OPAPKPATH/cmcc_march.zip
mv $OPAPKPATH/ikey.reinforce $OPAPKPATH/ikey2.reinforce
mv $OPAPKPATH/hmac.reinforce $OPAPKPATH/hmac2.reinforce
echo -e \\n

echo 第五步："开始加密classes.dex"
$OPAPKPATH/bin/encrypt $OPAPKPATH/cmcc_march.zip 4 $OPAPKPATH/cmcc_march
mv $OPAPKPATH/ikey.reinforce $OPAPKPATH/ikey_d.reinforce
echo -e \\n

echo 第六步："开始计算loader的hmac"
$OPAPKPATH/bin/hmacmaker $OPAPKPATH/classes.dex
mv $OPAPKPATH/ikey.reinforce $OPAPKPATH/ikey1.reinforce
mv $OPAPKPATH/hmac.reinforce $OPAPKPATH/hmac1.reinforce
echo -e \\n

echo 第七步："开始写白盒查找表"
$OPAPKPATH/bin/writetable
echo -e \\n

echo 第八步："开始加密白盒查找表"
cd $OPAPKPATH/
$OPAPKPATH/bin/encrypt $OPAPKPATH/WBkey.reinforce 5 WBkey1.reinforce
rm -rf $OPAPKPATH/WBkey.reinforce
mv $OPAPKPATH/WBkey1.reinforce $OPAPKPATH/WBkey.reinforce
mv $OPAPKPATH/ikey.reinforce $OPAPKPATH/ikey_ENC_WB.reinforce
echo -e \\n

echo 第九步："开始拼接"
$OPAPKPATH/bin/Add_wbkey_after_dex
mv $OPAPKPATH/cmcc_march $OPAPKPATH/app/assets/cmcc_march
echo -e \\n

echo 第十步："开始替换密钥"
if test -d "$OPAPKPATH/app/lib/armeabi" ; then
echo armeabi
replace_armeabi_so
fi
if test -d "$OPAPKPATH/app/lib/armeabi-v7a" ; then
echo armeabi-v7a
replace_armeabi_v7a_so
fi
if test -d "$OPAPKPATH/app/lib/x86" ; then
echo x86
replace_armeabi_x86_so
fi
if test -d "$OPAPKPATH/app/lib/mips" ; then
echo mips
replace_armeabi_mips_so
fi
echo -e \\n

echo 第十一步："开始修改AndroidManifest"
cp $OPAPKPATH/classes.dex $OPAPKPATH/app/classes.dex
mv $OPAPKPATH/app/AndroidManifest.xml $OPAPKPATH/tmp/AndroidManifest.xml
java -jar $OPAPKPATH/UpdateXml.jar $OPAPKPATH/tmp/AndroidManifest.xml
mv $OPAPKPATH/tmp/AndroidManifest.xml $OPAPKPATH/app/AndroidManifest.xml
# test
#cp $OPAPKPATH/app/AndroidManifest.xml $OPAPKPATH/AndroidManifest.xml
echo -e \\n

echo 第十二步："开始打包签名"
cd $OPAPKPATH/
java -jar $OPAPKPATH/tools/apktool.jar b $OPAPKPATH/app -o $OPAPKPATH/app.apk
java -jar $OPAPKPATH/tools/signapk.jar $OPAPKPATH/tools/testkey.x509.pem $OPAPKPATH/tools/testkey.pk8 $OPAPKPATH/app.apk $2

#Clean Job
rm -rf $OPAPKPATH/bin/*.reinforce
rm -rf $OPAPKPATH/*.reinforce
rm -rf $OPAPKPATH/tmp/armeabi/*
rm -rf $OPAPKPATH/tmp/armeabi-v7a/*
rm -rf $OPAPKPATH/tmp/x86/*
rm -rf $OPAPKPATH/tmp/mips/*
rm -rf $OPAPKPATH/tmp/*.apk
rm -rf $OPAPKPATH/app.apk
#rm -rf $OPAPKPATH/test.jar
rm -rf $OPAPKPATH/app/*
rm -rf $OPAPKPATH/app
rm -rf $OPAPKPATH/tmp/assets
rm -rf $OPAPKPATH/tmp/lib
rm -rf $OPAPKPATH/cmcc_march.zip
fi
