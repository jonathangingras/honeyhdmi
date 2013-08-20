honeyhdmi
=========

App to manage a remote X session via hdmi port on optimus laptops.

This project is intended to provide an easy GUI to run external display using one'S laptop's nVidia GPU.


<h3>Requirements:</h3>

-Bumblebee
-Synergy
-Openbox (only required for external display)
-Qt4
-xscreensaver(optional)

<h3>INSTALL</h3>

<code>
$ qmake-qt4 # or any equivalent qmake command <br>
$ make<br>
$ gcc ./obwrap/honeyhdmi_obwrap.c -o honeyhdmi_obwrap<br>
$ mv honeyhdmi_obwrap # somewhere in your $PATH<br>
$ mv hdmi #somewhere in your $PATH<br>
$ mv honeyhdmi #somewhere in your $PATH<br>
</code>
