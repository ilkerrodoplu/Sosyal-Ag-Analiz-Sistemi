#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAKS_KULLANICILAR 1000
#define KIRMIZI 0
#define SIYAH 1

typedef struct Kullanici {
    int id;
    int arkadasSayisi;
    struct Kullanici** arkadaslar;
} Kullanici;

typedef struct KSDugum {
    int id;
    Kullanici* kullanici;
    int renk;
    struct KSDugum* ebeveyn;
    struct KSDugum* sol;
    struct KSDugum* sag;
} KSDugum;

typedef struct Graf {
    Kullanici** kullanicilar;
    int kullaniciSayisi;
    KSDugum* ksKok;
} Graf;

// Bos dugum  icin global degisken
KSDugum* BOS;

void dfsAra(Kullanici* kullanici, int derinlik, int maksDerinlik, int* ziyaretEdildi);
void bfsTopluluklari(Kullanici* baslangicKullanici, int toplulukId, int* ziyaretEdildi);
void bfsEtkiAlani(Kullanici* baslangicKullanici, int* ziyaretEdildi, int* uzaklik);

Kullanici* kullaniciOlustur(int id) {
    Kullanici* yeniKullanici = (Kullanici*)malloc(sizeof(Kullanici));
    if (yeniKullanici) {
        yeniKullanici->id = id;
        yeniKullanici->arkadasSayisi = 0;
        yeniKullanici->arkadaslar = NULL;
    }
    return yeniKullanici;
}

KSDugum* ksDugumOlustur(int id, Kullanici* kullanici) {
    KSDugum* yeniDugum = (KSDugum*)malloc(sizeof(KSDugum));
    if (yeniDugum) {
        yeniDugum->id = id;
        yeniDugum->kullanici = kullanici;
        yeniDugum->renk = KIRMIZI;
        yeniDugum->ebeveyn = BOS;
        yeniDugum->sol = BOS;
        yeniDugum->sag = BOS;
    }
    return yeniDugum;
}

Graf* grafOlustur() {
    Graf* graf = (Graf*)malloc(sizeof(Graf));
    if (graf) {
        graf->kullanicilar = (Kullanici**)malloc(MAKS_KULLANICILAR * sizeof(Kullanici*));
        graf->kullaniciSayisi = 0;
        graf->ksKok = BOS;
        int i;
        for (i = 0; i < MAKS_KULLANICILAR; i++) {
            graf->kullanicilar[i] = NULL;
        }
    }
    return graf;
}

void solRotasyon(Graf* graf, KSDugum* x) {
    KSDugum* y = x->sag;
    x->sag = y->sol;

    if (y->sol != BOS) {
        y->sol->ebeveyn = x;
    }

    y->ebeveyn = x->ebeveyn;

    if (x->ebeveyn == BOS) {
        graf->ksKok = y;
    } else if (x == x->ebeveyn->sol) {
        x->ebeveyn->sol = y;
    } else {
        x->ebeveyn->sag = y;
    }

    y->sol = x;
    x->ebeveyn = y;
}

void sagRotasyon(Graf* graf, KSDugum* y) {
    KSDugum* x = y->sol;
    y->sol = x->sag;

    if (x->sag != BOS) {
        x->sag->ebeveyn = y;
    }

    x->ebeveyn = y->ebeveyn;

    if (y->ebeveyn == BOS) {
        graf->ksKok = x;
    } else if (y == y->ebeveyn->sol) {
        y->ebeveyn->sol = x;
    } else {
        y->ebeveyn->sag = x;
    }

    x->sag = y;
    y->ebeveyn = x;
}


void eklemeyiDuzelt(Graf* graf, KSDugum* k) {
    KSDugum* u;
    while (k->ebeveyn->renk == KIRMIZI) {
        if (k->ebeveyn == k->ebeveyn->ebeveyn->sag) {
            u = k->ebeveyn->ebeveyn->sol;
            if (u->renk == KIRMIZI) {
                u->renk = SIYAH;
                k->ebeveyn->renk = SIYAH;
                k->ebeveyn->ebeveyn->renk = KIRMIZI;
                k = k->ebeveyn->ebeveyn;
            } else {
                if (k == k->ebeveyn->sol) {
                    k = k->ebeveyn;
                    sagRotasyon(graf, k);
                }
                k->ebeveyn->renk = SIYAH;
                k->ebeveyn->ebeveyn->renk = KIRMIZI;
                solRotasyon(graf, k->ebeveyn->ebeveyn);
            }
        } else {
            u = k->ebeveyn->ebeveyn->sag;

            if (u->renk == KIRMIZI) {
                u->renk = SIYAH;
                k->ebeveyn->renk = SIYAH;
                k->ebeveyn->ebeveyn->renk = KIRMIZI;
                k = k->ebeveyn->ebeveyn;
            } else {
                if (k == k->ebeveyn->sag) {
                    k = k->ebeveyn;
                    solRotasyon(graf, k);
                }
                k->ebeveyn->renk = SIYAH;
                k->ebeveyn->ebeveyn->renk = KIRMIZI;
                sagRotasyon(graf, k->ebeveyn->ebeveyn);
            }
        }
        if (k == graf->ksKok) {
            break;
        }
    }
    graf->ksKok->renk = SIYAH;
}

void ksDugumEkle(Graf* graf, int id, Kullanici* kullanici) {
    KSDugum* yeniDugum = ksDugumOlustur(id, kullanici);
    KSDugum* y = BOS;
    KSDugum* x = graf->ksKok;

    while (x != BOS) {
        y = x;
        if (yeniDugum->id < x->id) {
            x = x->sol;
        } else {
            x = x->sag;
        }
    }

    yeniDugum->ebeveyn = y;

    if (y == BOS) {
        graf->ksKok = yeniDugum;
    } else if (yeniDugum->id < y->id) {
        y->sol = yeniDugum;
    } else {
        y->sag = yeniDugum;
    }

    if (yeniDugum->ebeveyn == BOS) {
        yeniDugum->renk = SIYAH;
        return;
    }

    if (yeniDugum->ebeveyn->ebeveyn == BOS) {
        return;
    }

    eklemeyiDuzelt(graf, yeniDugum);
}

Kullanici* kullaniciAraKS(KSDugum* kok, int id) {
    if (kok == BOS) {
        return NULL;
    }

    if (kok->id == id) {
        return kok->kullanici;
    }

    if (id < kok->id) {
        return kullaniciAraKS(kok->sol, id);
    }

    return kullaniciAraKS(kok->sag, id);
}

Kullanici* kullaniciAra(Graf* graf, int id) {
    return kullaniciAraKS(graf->ksKok, id);
}

void kullaniciEkle(Graf* graf, int id) {
    if (kullaniciAra(graf, id) != NULL) {
        printf("Kullanici USER %d zaten var!\n", id);
        return;
    }

    Kullanici* yeniKullanici = kullaniciOlustur(id);
    if (yeniKullanici) {
        graf->kullanicilar[graf->kullaniciSayisi++] = yeniKullanici;
        ksDugumEkle(graf, id, yeniKullanici);
        printf("Kullanici USER %d eklendi.\n", id);
    }
}

void arkadaslikEkle(Graf* graf, int id1, int id2) {
    Kullanici* kullanici1 = kullaniciAra(graf, id1);
    Kullanici* kullanici2 = kullaniciAra(graf, id2);

    if (!kullanici1 || !kullanici2) {
        printf("Bir veya her iki kullanici bulunamadi!\n");
        return;
    }

    int i;
    for (i = 0; i < kullanici1->arkadasSayisi; i++) {
        if (kullanici1->arkadaslar[i]->id == id2) {
            printf("USER %d ve USER %d zaten arkadas!\n", id1, id2);
            return;
        }
    }

    kullanici1->arkadaslar = (Kullanici**)realloc(kullanici1->arkadaslar, (kullanici1->arkadasSayisi + 1) * sizeof(Kullanici*));
    kullanici1->arkadaslar[kullanici1->arkadasSayisi++] = kullanici2;

    kullanici2->arkadaslar = (Kullanici**)realloc(kullanici2->arkadaslar, (kullanici2->arkadasSayisi + 1) * sizeof(Kullanici*));
    kullanici2->arkadaslar[kullanici2->arkadasSayisi++] = kullanici1;

    printf("USER %d ve USER %d arkadas oldular.\n", id1, id2);
}
//Derinlik oncelikli arama (Asagi dogru)
void dfsAra(Kullanici* kullanici, int derinlik, int maksDerinlik, int* ziyaretEdildi) {
    if (derinlik > maksDerinlik) return;
    if (!ziyaretEdildi[kullanici->id]) {
        ziyaretEdildi[kullanici->id] = 1;
        if (derinlik > 0 && derinlik <= maksDerinlik) {
            printf("Derinlik %d: USER %d\n", derinlik, kullanici->id);
        }
        if (derinlik < maksDerinlik) {
            int i;
            for (i = 0; i < kullanici->arkadasSayisi; i++) {
                Kullanici* arkadas = kullanici->arkadaslar[i];
                if (!ziyaretEdildi[arkadas->id]) {
                    dfsAra(arkadas, derinlik + 1, maksDerinlik, ziyaretEdildi);
                }
            }
        }
    }
}

void belirliMesafedeArkadaslariBul(Graf* graf, int baslangicId, int maksDerinlik) {
    Kullanici* baslangicKullanici = kullaniciAra(graf, baslangicId);
    if (!baslangicKullanici) {
        printf("Baslangic kullanicisi bulunamadi!\n");
        return;
    }

    int* ziyaretEdildi = (int*)calloc(MAKS_KULLANICILAR, sizeof(int));

    printf("USER %d icin %d derinligine kadar olan arkadaslar:\n", baslangicId, maksDerinlik);
    dfsAra(baslangicKullanici, 0, maksDerinlik, ziyaretEdildi);
    int sonucVar = 0;
    int i;
    for (i = 0; i < MAKS_KULLANICILAR; i++) {
        if (ziyaretEdildi[i] && i != baslangicId) {
            sonucVar = 1;
            break;
        }
    }
    if (!sonucVar) {
        printf("Bu derinlige kadar hic arkadas bulunamadi.\n");
    }
    free(ziyaretEdildi);
}

void ortakArkadaslariBul(Graf* graf, int id1, int id2) {
    Kullanici* kullanici1 = kullaniciAra(graf, id1);
    Kullanici* kullanici2 = kullaniciAra(graf, id2);

    if (!kullanici1 || !kullanici2) {
        printf("Bir veya her iki kullanici bulunamadi!\n");
        return;
    }
    printf("USER %d ve USER %d icin ortak arkadaslar:\n", id1, id2);
    int ortakSayi = 0;
    int i, j;
    for (i = 0; i < kullanici1->arkadasSayisi; i++) {
        Kullanici* arkadas1 = kullanici1->arkadaslar[i];

        for (j = 0; j < kullanici2->arkadasSayisi; j++) {
            if (arkadas1->id == kullanici2->arkadaslar[j]->id) {
                printf("- USER %d\n", arkadas1->id);
                ortakSayi++;
                break;
            }
        }
    }

    if (ortakSayi == 0) {
        printf("Ortak arkadas bulunamadi.\n");
    } else {
        printf("Toplam %d ortak arkadas bulundu.\n", ortakSayi);
    }
}
//Genislik oncelikli arama (Ayni seviye)
void bfsTopluluklari(Kullanici* baslangicKullanici, int toplulukId, int* ziyaretEdildi) {
    Kullanici** kuyruk = (Kullanici**)malloc(MAKS_KULLANICILAR * sizeof(Kullanici*));
    int on = 0, arka = 0;

    kuyruk[arka++] = baslangicKullanici;
    ziyaretEdildi[baslangicKullanici->id] = toplulukId;
    printf("Topluluk %d: USER %d", toplulukId, baslangicKullanici->id);

    while (on < arka) {
        Kullanici* kullanici = kuyruk[on++];
        int i;
        for (i = 0; i < kullanici->arkadasSayisi; i++) {
            Kullanici* arkadas = kullanici->arkadaslar[i];
            if (!ziyaretEdildi[arkadas->id]) {
                ziyaretEdildi[arkadas->id] = toplulukId;
                printf(", USER %d", arkadas->id);
                kuyruk[arka++] = arkadas;
            }
        }
    }

    printf("\n");
    free(kuyruk);
}

void topluluklariTespit(Graf* graf) {
    int* ziyaretEdildi = (int*)calloc(MAKS_KULLANICILAR, sizeof(int));
    int toplulukSayisi = 0;

    printf("Topluluk tespiti basliyor...\n");
    int i;
    for (i = 0; i < graf->kullaniciSayisi; i++) {
        Kullanici* kullanici = graf->kullanicilar[i];
        if (kullanici && !ziyaretEdildi[kullanici->id]) {
            toplulukSayisi++;
            bfsTopluluklari(kullanici, toplulukSayisi, ziyaretEdildi);
        }
    }

    printf("Toplam %d topluluk tespit edildi.\n", toplulukSayisi);
    free(ziyaretEdildi);
}
//Genislik oncelikli arama (Ayni seviye)
void bfsEtkiAlani(Kullanici* baslangicKullanici, int* ziyaretEdildi, int* uzaklik) {
    Kullanici** kuyruk = (Kullanici**)malloc(MAKS_KULLANICILAR * sizeof(Kullanici*));
    int on = 0, arka = 0;

    kuyruk[arka++] = baslangicKullanici;
    ziyaretEdildi[baslangicKullanici->id] = 1;
    uzaklik[baslangicKullanici->id] = 0;

    while (on < arka) {
        Kullanici* kullanici = kuyruk[on++];
        int i;
        for (i = 0; i < kullanici->arkadasSayisi; i++) {
            Kullanici* arkadas = kullanici->arkadaslar[i];
            if (!ziyaretEdildi[arkadas->id]) {
                ziyaretEdildi[arkadas->id] = 1;
                uzaklik[arkadas->id] = uzaklik[kullanici->id] + 1;
                kuyruk[arka++] = arkadas;
            }
        }
    }

    free(kuyruk);
}

void etkiAlaniHesapla(Graf* graf, int kullaniciId) {
    Kullanici* kullanici = kullaniciAra(graf, kullaniciId);
    if (!kullanici) {
        printf("Kullanici bulunamadi!\n");
        return;
    }

    int* ziyaretEdildi = (int*)calloc(MAKS_KULLANICILAR, sizeof(int));
    int* uzaklik = (int*)malloc(MAKS_KULLANICILAR * sizeof(int));
    int i;
    for (i = 0; i < MAKS_KULLANICILAR; i++) {
        uzaklik[i] = -1;
    }

    bfsEtkiAlani(kullanici, ziyaretEdildi, uzaklik);

    int maksUzaklik = 0;
    int ulasabilenKullanicilar = 0;
    int toplamUzaklik = 0;

    for (i = 0; i < graf->kullaniciSayisi; i++) {
        Kullanici* digerKullanici = graf->kullanicilar[i];
        if (digerKullanici && digerKullanici->id != kullaniciId && uzaklik[digerKullanici->id] != -1) {
            ulasabilenKullanicilar++;
            toplamUzaklik += uzaklik[digerKullanici->id];
            if (uzaklik[digerKullanici->id] > maksUzaklik) {
                maksUzaklik = uzaklik[digerKullanici->id];
            }
        }
    }

    float ortUzaklik = ulasabilenKullanicilar > 0 ? (float)toplamUzaklik / ulasabilenKullanicilar : 0;

    printf("\nUSER %d icin etki alani analizi:\n", kullaniciId);
    printf("- Direkt arkadas sayisi: %d\n", kullanici->arkadasSayisi);
    printf("- Ulasabildigi toplam kullanici sayisi: %d\n", ulasabilenKullanicilar);
    printf("- Maksimum ulasim mesafesi: %d\n", maksUzaklik);
    printf("- Ortalama ulasim mesafesi: %.2f\n", ortUzaklik);

    float etki = kullanici->arkadasSayisi * (ulasabilenKullanicilar > 0 ? (float)ulasabilenKullanicilar / maksUzaklik : 0);
    printf("- Etki puani: %.2f\n", etki);

    free(ziyaretEdildi);
    free(uzaklik);
}

void grafTemizle(Graf* graf) {
    int i;
    if (graf) {
        for (i = 0; i < graf->kullaniciSayisi; i++) {
            if (graf->kullanicilar[i]) {
                if (graf->kullanicilar[i]->arkadaslar) {
                    free(graf->kullanicilar[i]->arkadaslar);
                }
                free(graf->kullanicilar[i]);
            }
        }
        free(graf->kullanicilar);
        free(graf);
    }
}

void dosyayaKaydet(Graf* graf, const char* dosyaAdi) {
    FILE* dosya = fopen(dosyaAdi, "w");
    if (!dosya) {
        printf("Dosya acilamadi!\n");
        return;
    }

    fprintf(dosya, "# Kullanicilar\n");
    int i, j;
    for (i = 0; i < graf->kullaniciSayisi; i++) {
        fprintf(dosya, "USER %d\n", graf->kullanicilar[i]->id);
    }

    fprintf(dosya, "\n# Arkadaslik iliskileri\n");
    for (i = 0; i < graf->kullaniciSayisi; i++) {
        Kullanici* kullanici = graf->kullanicilar[i];
        for (j = 0; j < kullanici->arkadasSayisi; j++) {
            if (kullanici->id < kullanici->arkadaslar[j]->id) {
                fprintf(dosya, "FRIEND %d %d\n", kullanici->id, kullanici->arkadaslar[j]->id);
            }
        }
    }

    fclose(dosya);
    printf("Veriler '%s' dosyasina kaydedildi.\n", dosyaAdi);
}

int dosyadanOku(Graf* graf, const char* dosyaAdi) {
    FILE* dosya = fopen(dosyaAdi, "r");
    if (!dosya) {
        return 0;
    }

    char satir[256];
    while (fgets(satir, sizeof(satir), dosya)) {
        if (satir[0] == '#' || satir[0] == '\n') {
            continue;
        }

        char komut[10];
        int id1, id2;

        if (sscanf(satir, "%9s %d %d", komut, &id1, &id2) >= 2) {
            if (strcmp(komut, "USER") == 0) {
                kullaniciEkle(graf, id1);
            } else if (strcmp(komut, "FRIEND") == 0 && sscanf(satir, "%9s %d %d", komut, &id1, &id2) == 3) {
                arkadaslikEkle(graf, id1, id2);
            }
        }
    }

    fclose(dosya);
    printf("'%s' dosyasindaki veriler yuklendi.\n", dosyaAdi);
    return 1;
}

int main() {
    BOS = (KSDugum*)malloc(sizeof(KSDugum));
    BOS->renk = SIYAH;
    BOS->sol = NULL;
    BOS->sag = NULL;
    BOS->ebeveyn = NULL;

    Graf* graf = grafOlustur();
    char girdi[100];
    int kullaniciId, arkadasId, maksDerinlik;
    const char* dosyaAdi = "veriseti.txt";

    printf("Sosyal Ag Analiz Programina Hosgeldiniz\n");
    printf("--------------------------------------\n");

    int dosyaBulundu = dosyadanOku(graf, dosyaAdi);

    if (dosyaBulundu) {
        printf("\nMevcut verilere ekleme yapmak istiyor musunuz? (E/H): ");
        fgets(girdi, sizeof(girdi), stdin);

        if (girdi[0] != 'E' && girdi[0] != 'e') {
            goto menu;
        }
    }

    while (1) {
        printf("Eklemek istediginiz kullanici ID'sini girin (cikmak icin 'B' veya 'b'): ");
        if (fgets(girdi, sizeof(girdi), stdin) == NULL) {
            break;
        }

        if (girdi[0] == 'B' || girdi[0] == 'b') {
            break;
        }
        int id;
        if (sscanf(girdi, "%d", &id) != 1) {
            printf("Gecersiz ID formati! Lutfen bir sayi girin.\n");
            continue;
        }
        kullaniciEkle(graf, id);
    }

    printf("\nSimdi arkadas baglarini ekleyin (Cikmak icin 'B' veya 'b' girin):\n");

    while (1) {
        printf("Arkadas bagini 'USER_ID1 USER_ID2' formatinda girin (Cikmak icin 'B' veya 'b'): ");
        if (fgets(girdi, sizeof(girdi), stdin) == NULL) {
            break;
        }

        if (girdi[0] == '\n' || girdi[0] == 'B' || girdi[0] == 'b') {
            if (girdi[0] != '\n') break;
            continue;
        }
        int id1, id2, okunanSayi;
        okunanSayi = sscanf(girdi, "%d %d", &id1, &id2);

        if (okunanSayi != 2) {
            printf("Gecersiz format! Lutfen 'USER_ID1 USER_ID2' formatinda girin.\n");
            continue;
        }
        Kullanici* kullanici1 = kullaniciAra(graf, id1);
        Kullanici* kullanici2 = kullaniciAra(graf, id2);

        if (!kullanici1) {
            printf("HATA: USER %d bulunamadi!\n", id1);
            continue;
        }

        if (!kullanici2) {
            printf("HATA: USER %d bulunamadi!\n", id2);
            continue;
        }

        arkadaslikEkle(graf, id1, id2);
    }
    dosyayaKaydet(graf, dosyaAdi);
menu:;
    int secim;
    do {
        printf("\n===== MENU =====\n");
        printf("1. Belirli mesafedeki arkadaslari bul (DFS)\n");
        printf("2. Ortak arkadas analizi\n");
        printf("3. Topluluk tespiti\n");
        printf("4. Etki alani hesapla\n");
        printf("5. Cikis\n");
        printf("Seciminiz: ");
        if (scanf("%d", &secim) != 1) {
            while (getchar() != '\n');
            printf("Lutfen bir sayi girin!\n");
            continue;
        }
        switch (secim) {
            case 1:
                printf("Baslangic kullanicisinin ID'sini girin: ");
                if (scanf("%d", &kullaniciId) != 1) {
                    printf("Gecersiz ID!\n");
                    while (getchar() != '\n');
                    break;
                }
                printf("Istenen derinligi girin: ");
                if (scanf("%d", &maksDerinlik) != 1) {
                    printf("Gecersiz derinlik!\n");
                    while (getchar() != '\n');
                    break;
                }
                belirliMesafedeArkadaslariBul(graf, kullaniciId, maksDerinlik);
                break;
            case 2:
                printf("Birinci kullanicinin ID'sini girin: ");
                if (scanf("%d", &kullaniciId) != 1) {
                    printf("Gecersiz ID!\n");
                    while (getchar() != '\n');
                    break;
                }
                printf("Ikinci kullanicinin ID'sini girin: ");
                if (scanf("%d", &arkadasId) != 1) {
                    printf("Gecersiz ID!\n");
                    while (getchar() != '\n');
                    break;
                }
                ortakArkadaslariBul(graf, kullaniciId, arkadasId);
                break;
            case 3:
                topluluklariTespit(graf);
                break;

            case 4:
                printf("Etki alani hesaplanacak kullanicinin ID'sini girin: ");
                if (scanf("%d", &kullaniciId) != 1) {
                    printf("Gecersiz ID!\n");
                    while (getchar() != '\n');
                    break;
                }
                etkiAlaniHesapla(graf, kullaniciId);
                break;
            case 5:
                printf("Programdan cikiliyor...\n");
                break;

            default:
                printf("Gecersiz secim!\n");
                break;
        }
        while (getchar() != '\n');

    } while (secim != 5);
    grafTemizle(graf);
    free(BOS);
    return 0;
}
