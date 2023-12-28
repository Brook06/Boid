# visualizer.py

Prima di utilizzare visualizer.py è necessario installare Python

Successivamente è necessario installare numpy e matplotlib (se non già installati) con:

`pip install numpy`

`pip install matplotlib`

Nel caso non funzioni (tipicamente su windows) una possibile soluzione è:

`py -m pip install numpy`

`py -m pip install matplotlib`

### Avviare il visualizzatore

Spostare il file **visualizer.py** dove viene generato il file con le coordinate dei robot. Tale file dovrà essere rinominato **coordinates.txt**.

Poi da terminale:

`python visualizer.py`

oppure:

`py visualizer.py`

Sempre nel terminale indicare l'intervallo temporale (espresso in millisecondi) che si desidera avere tra la visualizzazione di un frame ed il successivo.

## GUI per python su WSL

Da PowerShell:

> wsl --version

Se versione 1:

> wsl --update

> wsl --shutdown

( <https://learn.microsoft.com/en-us/windows/wsl/tutorials/gui-apps> )

Se versione 2, passare a wsl, dal terminale linux:

`$ sudo apt install python3`

`$ sudo apt install python3-tk`

(The standard Python interface to the Tcl/Tk GUI toolkit)

`$ sudo apt install python3-pip`

(The package installer for Python)
