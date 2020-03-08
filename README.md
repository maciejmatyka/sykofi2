# Symulacje Komputerowe w Fizyce, wydanie 2

Zbiór kodów źródłowych, programów, ilustracji i animacji do książki
[Maciej Matyka, "Symulacje Komputerowe w Fizyce", wydanie 2, Helion 2020](https://helion.pl/ksiazki/symulacje-komputerowe-w-fizyce-wydanie-ii-maciej-matyka,sykof2.htm#format/d)
Kompletna lista materiałów z opisem znajduje się w książce.

*Zbiór kodów źródłowych (do wydania drugiego):*

<img align="right" width="140" height="140" src="Ilustracje/Wydanie2/cloth.jpg">

* [**KodyZrodlowe/Rozdzial2/cloth.html**](KodyZrodlowe/Rozdzial2/cloth.html)
  * Tkanina złożona z punktów i połączeń sprężystych, możliwość
interakcji z użyciem myszy, metoda Verlet całkowania równań różniczkowych
ruchu, kod i wizualizacja w HTML5

<img align="right" width="140" height="140" src="Ilustracje/Wydanie2/softbody.jpg">

* [**KodyZrodlowe/Rozdzial2/softbody.html**](KodyZrodlowe/Rozdzial2/softbody.html)
  * Ciało miękkie 2D (punkty, sprężyny i siła ciśnienia), kod i wizualizacja w HTML5.

<img align="right" width="140" height="140" src="Ilustracje/Wydanie2/konstruktor.jpg">
  
* [**KodyZrodlowe/Rozdzial2/konstruktor/konstruktor.html**](KodyZrodlowe/Rozdzial2/konstruktor/konstruktor.html)
  * Replika programu ["Soda Constructor"](https://en.wikipedia.org/wiki/Soda_Constructor) - umożliwia budowanie 
własnych obiektów z dodatkowymi sprężynami, których długość w stanie równowagi jest zależna od czasu (mięśnie)

<img align="right" width="140" height="140" src="Ilustracje/Wydanie2/softbody3d.jpg">
  
* [**KodyZrodlowe/Rozdzial2/SoftBody3D**](KodyZrodlowe/Rozdzial2/SoftBody3D)
  * Implementacja C++ trójwymiarowego [modelu ciała miękkiego](http://www.ift.uni.wroc.pl/~maq/papers/matyka03.pdf) opartego o ciśnienie. 
  Program wykorzystuje OpenGL i pozwala na symulację w czasie rzeczywistym. 
  Niektóre z obiektów (ręka, koala) wykonał Mariusz Jarosz (dziękuję!).


<img align="right" width="140" height="140" src="Ilustracje/Wydanie2/lbm.jpg">

* [**KodyZrodlowe/Rozdzial4/lbmfluid_book/**](KodyZrodlowe/Rozdzial4/lbmfluid_book/)
  * Implementacja C/C++ modelu Gazu Sieciowego Boltzmanna LBM w 
  wersji jednofazowej z komentarzami odpowiadającymi układowi w rozdziale z książki. Jest to prosta implementacja modelu
  jednorelaksacyjnego z wizualizacją w OpenGL i biblioteką GLUT. Dodatkowo do kodu dołączone są procedury wizualizacji
  poprzez śledzenie ruchu cząsteczek płynu i ich rozsmarowywanie. Uwaga: możliwe są problemy z podwójnym buforowaniem na niektórych
  systemach operacyjnych, wtedy należy dokładniej przyjrzeć się sterownikom i działaniu OpenGL.

<img align="right" width="140" height="140" src="Ilustracje/Wydanie2/lbm3D.jpg">

* [**KodyZrodlowe/Rozdzial4/lbmfluid3D**](KodyZrodlowe/Rozdzial4/lbmfluid3D)
  * Model [LBM](https://en.wikipedia.org/wiki/Lattice_Boltzmann_methods) w wersji trójwymiarowej, przybliżenie jednorelaksacyjne BGK.
  

<img align="right" width="140" height="140" src="Ilustracje/Wydanie2/drivencavitymrt.jpg">

* [**KodyZrodlowe/Rozdzial4/lbmfluid-mrt/**](KodyZrodlowe/Rozdzial4/lbmfluid-mrt/)
  * Przepływ w komorze (ang. Driven Cavity Flow). Implementacja modelu wielorelaksacyjnego (MRT) przepływu jednofazowego 2D z użyciem LBM dla 
  wysokich [liczb Reynoldsa](https://pl.wikipedia.org/wiki/Liczba_Reynoldsa).

  
<img align="right" width="140" height="140" src="Ilustracje/Wydanie2/lbmmulti.jpg">

* [**KodyZrodlowe/Rozdzial4/lbmfluid2D-multiphase/**](KodyZrodlowe/Rozdzial4/lbmfluid2D-multiphase/)
  * Implementacja przepływu wielofazowego 2D z użyciem [LBM](https://en.wikipedia.org/wiki/Lattice_Boltzmann_methods) i modelu Shan-Chen omawianego w
  książce.

<img align="right" width="140" height="140" src="Ilustracje/Wydanie2/lbmmulti3D.jpg">

* [**KodyZrodlowe/Rozdzial4/lbmfluid3D-multiphase/**](KodyZrodlowe/Rozdzial4/lbmfluid3D-multiphase/)
  * Implementacja przepływu wielofazowego 3D z użyciem [LBM](https://en.wikipedia.org/wiki/Lattice_Boltzmann_methods) i modelu Shan-Chen omawianego w
  książce.

<img align="right" width="140" height="140" src="Ilustracje/Wydanie2/lbmtau1.jpg">

* [**KodyZrodlowe/Rozdzial4/lbmfluid-tauone3D/**](KodyZrodlowe/Rozdzial4/lbmfluid-tauone3D/)
  * Implementacja modelu LBM bez funckcji rozkładu prędkości w wersji 3D i 2D (model [LBMTAU1](https://arxiv.org/abs/1912.09327)).

  
<img align="right" width="140" height="140" src="Ilustracje/Wydanie2/fractal.jpg">

* [**Kodyzrodlowe/Bonus/81fractalifs/fractalifs.cpp**](Kodyzrodlowe/Bonus/81fractalifs/fractalifs.cpp)
  * Program C/C++ generujący fraktal typu IFS (paproć). Kod wykonany na podstawie książki J. Kudrewicz, Fraktale i chaos, WNT 2015.
  Program generuje plik graficzny typu .ppm (Portable Pixel Map) - prosty format graficzny w trybie ASCII omówiony w książce w rozdziale o symulacji płynów.

<img align="right" width="140" height="140" src="Ilustracje/Wydanie2/Ising.jpg">

* [**Kodyzrodlowe/Bonus/82IsingModel/ising.cpp**](Kodyzrodlowe/Bonus/82IsingModel/ising.cpp)
  * Implementacja modelu sieciowego magnetyzmu (model Isinga), program w C++ z wizualizacją w OpenGL. Program pozwala na obserowanie 
  ewolucji układu spinów w czasie dla różnej temperatury.

  <img align="right" width="140" height="140" src="Ilustracje/Wydanie2/ripples.jpg">

* [**Kodyzrodlowe/Bonus/ripple2d.html**](Kodyzrodlowe/Bonus/ripple2d.html)
  * Prosty automat komórkowy generujący rozchodzenie się fal w dwóch wymiarach. Kod w HTML5 z wizualizacją w przeglądarce.

    <img align="right" width="140" height="140" src="Ilustracje/Wydanie2/dla.jpg">

* [**Kodyzrodlowe/Bonus/ripple2d.html**](Kodyzrodlowe/Bonus/dla.html)
  * Tworzenie struktur - fraktali - DLA ([Diffusion Limited Aggregation](https://en.wikipedia.org/wiki/Diffusion-limited_aggregation)) - bardzo proste, ale ciekawe zjawisko fizyczne i prosty model służący do jego symulacji. Program w JS/HTML5, wizualizacja w czasie rzeczywistym. Interakcja myszą pozwala na dokładanie nowych zarodków fraktala.

  <img align="right" width="140" height="140" src="Ilustracje/Wydanie2/palabos.jpg">

* [**Kodyzrodlowe/Bonus/PalabosCG/**](Kodyzrodlowe/Bonus/PalabosCG/)
  * Skrypt do biblioteki [Palabos](https://palabos.unige.ch/) umożliwiający rendering 3D z użyciem RayTracingu (PovRay) powierzchni swobodnej płynu.

[(c) Maciej Matyka, 2020](http://panoramix.ift.uni.wroc.pl/~maq/eng/)

