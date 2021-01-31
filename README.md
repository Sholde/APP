# APP - Algorithmique et Programmation Parallèle

Initiation à la programmation distribuée avec MPI et à la programmation à mémoire partagée avec les threads POSIX et OpenMP.

## Installation de MPI sur votre ordinateur

Télécharger `mpich-3.4.1.tar.gz` à l'adresse http://www.mpich.org/static/downloads/3.4.1/mpich-3.4.1.tar.gz
```
cd /tmp
tar xvzf /votre/repertoire/de/telechargement/mpich-3.4.1.tar.gz 
```

Le dossier créé est le répertoire de compilation que l'on pourra détruire après installation définitive.

Configuration :

```
cd mpich-3.4.1
./configure --prefix=/usr/local/mpich-3.4.1 --disable-cxx --disable-fortran --disable-romio --enable-threads=runtime --with-device=ch3
```

Le répertoire désigné par `--prefix=` sera le répertoire d'installation définitive, le chemin donné ici n'est qu'un exemple.

Compilation (plusieurs minutes) :

```
make
```

Installation (en fonction du répertoire final d'installation peut nécessiter les droits root) :

```
sudo make install
```

## Environnement, compilation, exécution

Préparation environnement (à exécuter dans chaque nouveau shell ou à mettre dans votre `.bashrc`) :
```
export PATH=/usr/local/mpich-3.4.1/bin:$PATH
```

Compilation d'un programme `monprog.c` (`mpicc` se comporte comme un compilateur classique) :
```
prompt> mpicc monprog.c -o monprog.exe
```

On execute autant de fois que nécessaire le programme MPI avec `mpiexec` :
```
prompt> mpiexec -n 4 ./monprog.exe
prompt> mpiexec -n 2 ./monprog.exe
prompt> etc ...
```

