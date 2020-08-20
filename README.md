# Termômetro Digital

Projeto de um termômetro digital baseado em Arduino com filtros digitais embarcados e interface em Matlab para a disciplina de [Sistemas de Medição](https://matriculaweb.unb.br/graduacao/disciplina.aspx?cod=168742) na UnB ministrada pelo professor Dr. Carlos Humberto Llanos.

O Matlab foi utilizado exclusivamente para a interface gráfica e foram implementados os filtros de média, média móvel e média exponêncialmente ponderada bem como um filtro de Kalman diretamente no Arduino. Dados do Termopar calibrados em laboratório utilizando um voltimetro e uma mistura de água e gelo.

## Requisitos

### Software
 - [Matlab](https://www.mathworks.com/products/matlab/)
 - [IDE Arduino](https://www.arduino.cc/en/Main/Software)

### Hardware
 - [Arduino](https://www.arduino.cc/en/Main/ArduinoBoardUno)
 - [LM35](https://github.com/akafael/sismed_termopar/raw/master/doc/lm35.pdf): sensor de temperatura
 - [MTK01](http://www.minipa.com.br/8/41/82-Minipa-Termopares-Tipo-K-MTK-01): termopar
 - [LM324](https://github.com/akafael/sismed_termopar/raw/master/doc/lm2902-n.pdf): Amplificador Operacional
 - Resistores: 100 ohms e 100 kohmns
 - Fio
 
## Referências
 - https://www.analog.com/media/cn/training-seminars/design-handbooks/temperature_sensors_chapter7.pdf?doc=CN0172.pdf
 - https://archive.org/details/measurementsyste00doeb/
