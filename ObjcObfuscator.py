import sys
import os
KnownList=dict()
BD="/Users/Naville/Desktop/llvm/build"
def ObfuscateAtPath(Path):
	os.system(BD+"/bin/clang -x objective-c -fsyntax-only -Xclang \
	-load -Xclang $BD/lib/ObjcObfuscator.dylib \
	-Xclang -plugin -Xclang -ObjcClassGuard "+Path+\
	">>./"+os.path.basename(os.path.normpath(Path))+".h")

def removeDuplicate(Path):
	f=open(Path,"r")
	lines=f.readlines()
	f.close()
	f=open(Path,"w")
	for x in lines:
		y=x.split(" ")[1]
		print y
		KnownList[y]=x.split(" ")[2]
	for Q in KnownList.keys():
		f.write('#define '+Q+" "+KnownList[Q])
	f.close()
if __name__ == '__main__':
	i=1
	while i< len(sys.argv):
		ObfuscateAtPath(sys.argv[i])
		removeDuplicate("./"+os.path.basename(os.path.normpath(sys.argv[i]))+".h")
		i+=1

