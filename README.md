# Symulacje Komputerowe w Fizyce 2

Zbiór kodów źródłowych do książki
[Maciej Matyka, "Symulacje Komputerowe w Fizyce", wydanie 2, Helion 2020](https://helion.pl/ksiazki/symulacje-komputerowe-w-fizyce-wydanie-ii-maciej-matyka,sykof2.htm#format/d)

*Lista kodów źródłowych:*

<img align="right" width="140" height="140" src="_IMG/cloth.jpg">

* [**Rozdzial2/cloth.html**](Rozdzial2/cloth.html)
  * tkanina złożona z punktów i połączeń sprężystych, możliwość
interakcji z użyciem myszy, metoda Verlet całkowania równań różniczkowych
ruchu, kod i wizualizacja w HTML5

<img align="right" width="140" height="140" src="_IMG/softbody.jpg">

* [**Rozdzial2/softbody.html**](Rozdzial2/softbody.html)
  * Ciało miękkie 2D (punkty, sprężyny i siła ciśnienia), kod i wizualizacja w HTML5.

<img align="right" width="140" height="140" src="_IMG/konstruktor.jpg">
  
* [**Rozdzial2/konstruktor/konstruktor.html**](Rozdzial2/konstruktor/konstruktor.html)
  * Replika programu ["Soda Constructor"](https://en.wikipedia.org/wiki/Soda_Constructor) - umożliwia budowanie 
własnych obiektów z dodatkowymi sprężynami, których długość w stanie równowagi jest zależna od czasu (mięśnie)

<img align="right" width="140" height="140" src="_IMG/lbm.jpg">

* [**Rozdzial4/lbmfluid_book/**](Rozdzial4/lbmfluid_book/)
  * implementacja C/C++ modelu [Gazu Sieciowego Boltzmanna LBM](https://en.wikipedia.org/wiki/Lattice_Boltzmann_methods) w 
  wersji jednofazowej z komentarzami odpowiadającymi układowi w rozdziale z książki. Jest to prosta implementacja modelu
  jednorelaksacyjnego z wizualizacją w OpenGL i biblioteką GLUT. Dodatkowo do kodu dołączone są procedury wizualizacji
  poprzez śledzenie ruchu cząsteczek płynu i ich rozsmarowywanie. Uwaga: możliwe są problemy z podwójnym buforowaniem na niektórych
  systemach operacyjnych, wtedy należy dokładniej przyjrzeć się sterownikom i działaniu OpenGL.

<img align="right" width="140" height="140" src="_IMG/lbm3D.jpg">

* [**Rozdzial4/lbmfluid3D**](Rozdzial4/lbmfluid3D)
  * Model [LBM](https://en.wikipedia.org/wiki/Lattice_Boltzmann_methods) w wersji trójwymiarowej.
  
  
<img align="right" width="140" height="140" src="_IMG/lbmmulti.jpg">

* [**Rozdzial4/lbmfluid2D-multiphase/**](Rozdzial4/lbmfluid2D-multiphase/)
  * Implementacja przepływu wielofazowego 2D z użyciem [LBM](https://en.wikipedia.org/wiki/Lattice_Boltzmann_methods) i modelu Shan-Chen omawianego w
  książce.

<img align="right" width="140" height="140" src="_IMG/lbmmulti3D.jpg">

* [**Rozdzial4/lbmfluid3D-multiphase/**](Rozdzial4/lbmfluid3D-multiphase/)
  * Implementacja przepływu wielofazowego 3D z użyciem [LBM](https://en.wikipedia.org/wiki/Lattice_Boltzmann_methods) i modelu Shan-Chen omawianego w
  książce.
  
<img align="right" width="140" height="140" src="_IMG/fractal.jpg">

* [**Bonus/81fractalifs/fractalifs.cpp**](Bonus/81fractalifs/fractalifs.cpp)
  * Program C/C++ generujący fraktal typu IFS (paproć). Kod wykonany na podstawie książki J. Kudrewicz, Fraktale i chaos, WNT 2015.
  Program generuje plik graficzny typu .ppm (Portable Pixel Map) - prosty format graficzny w trybie ASCII omówiony w książce w rozdziale o symulacji płynów.

<img align="right" width="140" height="140" src="_IMG/Ising.jpg">

* [**Bonus/82IsingModel/ising.cpp**](Bonus/82IsingModel/ising.cpp)
  * Implementacja modelu sieciowego magnetyzmu (model Isinga), program w C++ z wizualizacją w OpenGL. Program pozwala na obserowanie 
  ewolucji układu spinów w czasie dla różnej temperatury.

  <img align="right" width="140" height="140" src="_IMG/ripples.jpg">

* [**Bonus/ripple2d.html**](Bonus/ripple2d.html)
  * Prosty automat komórkowy generujący rozchodzenie się fal w dwóch wymiarach. Kod w HTML5 z wizualizacją w przeglądarce.

[(c) Maciej Matyka, 2020](http://panoramix.ift.uni.wroc.pl/~maq/eng/)

