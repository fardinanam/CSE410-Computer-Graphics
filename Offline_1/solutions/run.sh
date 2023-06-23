if [ $# -ne 1 ] || [[ ${1##*.} != "cpp" ]]; then
  echo "usage: run.sh <cpp_file_name>" & exit 1
fi

fileName="${1%.*}"
g++ "$1" -o "$fileName" -lglut -lGLU -lGL
./"$fileName"