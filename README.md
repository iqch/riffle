# Introduction #

Rif Riffler is a dso module, that allows using custom python scripts act like rib-filtering procedure.

Rif Riffler - расширение renderman-совмесtимых рендереров для использования python-скриптов для целей фильтрации rib-потока.

# Preface #

Authoring of Rib-filters now is sufficient complicated procedure, notwithstanding that these types of renderman-compatible renderer's extensions have wide range of use.

This complexity comes from native-code based nature of rib-filters idea, so you have to write C/C++ source-code, compile and link it to get working rib-filter, which is just common dso. Compilation process is not too complicated and tricky, but it may to  frighten off some TD's, whose have no good experience with C/C++ programming. Moreover, sometime it is no good to allow some native code to run anywhere, so system administrators may block any untested or untrusted dso modules. Using scripting-based languages is widespread in TD's community, so, if there will be possibility to write filering scripts with widely used languages, like python, perl or jscript, it will allow using of rib-filtering possibility much more.
We decide to allow using python-based scripting procedures due to widest prevalence of python in CG-world.

This time Pixar's Renderman Pro-Server allows to write rib-filtering procedures with python for using them during python-oriented rib-processing, authoring or rendering. But most RIB-oriented pipelines are not python-centric, so, using those possibilities is not suitable for them.


Написание Rib-фильтров сегодня - непростая процедура, несмотря на то, что такие расширения имеют очень широкий спектр применения.
Прежде всего, это связано с тем, что обычно Rib-фильры - это dso, которые необходимо программировать на языках С/С++, с последующей компиляцией в исполняемый код.
Сама процедура компиляции не такая уж и сложная, но всё-таки она может отпугнуть менее профессионального пользователя, не имеющего опыта в такой разработке. Опыт использования скриптовых языков распространён гораздо шире, и такие языки, как python, perl, javascript активно используются профессионалами CG. Кроме того, удобство правки кода, отсутсвие (в явном виде) дополнительных шагов по компиляции/сборке также могут расцениваться как приемущества даже для очень и очень опытных разработчиков.
Поэтому, предоставить возможность пользователям исполнять python-код в качестве фильтрующей процедуры для Renderman-совместимых рендереров кажется вполне перспективной идеей.
На настоящий момент в python-поддержке для Pixar's Renderman Pro Server версии 14.+ имеется простая возможность фильтровать Rib-поток, генерируемыйс с помощью python-скрипта. Но этот подход ограничен созданием rib-потока с помощью python вызовов, что происходит не всегда, чаще пользователи имеют в своём распоряжении не-python-ориентированные средства rib-авторинга.
