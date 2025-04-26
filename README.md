# 📈 Sosyal Ağ Analizi Programı

Bu C dili projesi, bir sosyal ağ üzerinde kullanıcılar ve arkadaşlık ilişkilerini yönetmek ve analiz etmek için geliştirilmiştir. 
Program, kullanıcılar arasında arkadaşlıklar kurar, topluluklar keşfeder ve kullanıcıların etki alanlarını hesaplar.

## Özellikler

-  Kullanıcı ekleme ve arkadaşlık ilişkisi kurma
-  Dosyadan veri okuma ve kaydetme (veriseti.txt)
-  Belirli mesafedeki arkadaşları bulma (DFS)
-  Ortak arkadaş analizi
-  Topluluk (bağlantılı bileşen) tespiti
-  Kullanıcı etki alanı ve etki puanı hesaplama

## Kullanım

Programı çalıştırdığınızda:

1. Var olan bir veri seti yüklenir (eğer varsa).
2. İsterseniz yeni kullanıcı ve arkadaşlıklar eklersiniz.
3. Menüden analiz seçeneklerini kullanabilirsiniz.

## NASIL ÇALIŞTIRILIR?
İnternet üzerinden uygulamaya gerek olmadan çalışan bir derleyici: [Online C Compiler](https://www.onlinegdb.com/online_c_compiler)

Windows bilgisayarda çalışması için: [DevC++(Direk indirme sayfasına atar)](https://sourceforge.net/projects/dev-cpp/files/Binaries/Dev-C%2B%2B%204.9.9.2/devcpp-4.9.9.2_setup.exe/download) 

Tüm bilgisayarlarda çalışması için: [Visual Studio](https://code.visualstudio.com/Download)

## Veri Formatı (veriseti.txt)

```plaintext
# Kullanicilar
USER 1
USER 2
USER 3

# Arkadaslik iliskileri
FRIEND 1 2
FRIEND 2 3
