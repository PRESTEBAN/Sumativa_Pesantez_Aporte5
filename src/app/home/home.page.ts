import { Component, OnInit } from '@angular/core';
import { Database, object, ref } from '@angular/fire/database';
import { LocalNotifications } from '@capacitor/local-notifications';

@Component({
  selector: 'app-home',
  templateUrl: 'home.page.html',
  styleUrls: ['home.page.scss'],
})
export class HomePage implements OnInit {

  nivelLight: number = 0;
  isDaytime: boolean = true;

  constructor(private database: Database) {}

  ngOnInit() {
    const route = ref(this.database, "/Control luz/ValueLight");
    object(route).subscribe(attributes => {
      const valores_db = attributes.snapshot.val();
      console.log(valores_db);
      this.nivelLight = valores_db;
      this.verificarEstadoDiaNoche();
    });
  }

  async verificarEstadoDiaNoche() {
    if (this.nivelLight >= 0 && this.nivelLight <= 1500 && this.isDaytime) {
      this.notificacion("Es de noche.");
      this.isDaytime = false;
    } else if (this.nivelLight >= 1501 && this.nivelLight <= 4095 && !this.isDaytime) {
      this.notificacion("Es de día.");
      this.isDaytime = true;
    }
  }

  async notificacion(mensaje: string) {
    await LocalNotifications.requestPermissions();
    await LocalNotifications.schedule({
      notifications: [
        {
          title: "Estado de iluminación",
          body: mensaje,
          id: 1
        }
      ]
    });
  }
}
