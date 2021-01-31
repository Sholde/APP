# APP - Algorithmique et Programmation Parallèle

Initiation à la programmation distribuée avec MPI et à la programmation à mémoire partagée avec les threads POSIX et OpenMP.

## Installation de MPI sur votre ordinateur

Télécharger `mpich-3.2.1.tar.gz` à l'adresse http://www.mpich.org/static/downloads/3.2.1/mpich-3.2.1.tar.gz
```
cd /tmp
tar xvzf /votre/repertoire/de/telechargement/mpich-3.2.1.tar.gz 
```

Le dossier créé est le répertoire de compilation que l'on pourra détruire après installation définitive.

```
cd mpich-3.2.1
```

Configuration :

```
./configure --prefix=/usr/local/mpich-3.2.1 --disable-cxx --disable-fortran --disable-romio --enable-threads=runtime
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

