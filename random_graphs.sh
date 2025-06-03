mkdir -p out
mkdir -p bin
mkdir -p tex_files
rm tex_files/*
rm out/*
g++ src/Generator.cpp -o bin/generator
for ((i=1;i<=10;i++))
do
	./bin/generator "$i" "tex_files/$i.tex" "tex_files/${i}_planar.tex"
done
for file in tex_files/*.tex; do
    pdflatex -interaction=batchmode -output-directory=out "$file"
done

rm out/*.log
rm out/*.aux
