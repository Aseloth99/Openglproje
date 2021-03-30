#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

#define Simit 0	//simit tanımlama 0 olarak. Henüz belirtilmedi 
#define Caydanlik 1
#define cikis 5

static int spinx = 0;
static int obj = Simit;  //ön tanımlı olarak simit seçtim
static int beginx;

GLfloat yakuz = -4.0; //sahneye olan yakınlık uzaklık

GLfloat TX;			//x ekseninde döndürme
GLfloat TY;			//y ekseninde döndürme

GLfloat angle = 45.0;   //dönüş açısını 45 olarak belirleme

//diffuse light color 
GLfloat dlr = 1.0;
GLfloat dlg = 1.0;
GLfloat dlb = 1.0;

//ambient light color 
GLfloat alr = 1.0;
GLfloat alg = 1.0;
GLfloat alb = 1.0;

//light position 
GLfloat lx = 0.0;
GLfloat ly = 0.0;
GLfloat lz = 1.0;
GLfloat lw = 0.0;

void menu_select(int item)	 //sağ tıkladığımızda çıkan menü
{
	if (item == cikis)		//çıkış yapar
		exit(0);
	obj = item;			//İtemi seçer
	glutPostRedisplay();
}

void movelight(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {		//fare ile ışığı x ekseninde hareket ettirme
		beginx = x;
	}
}

void motion(int x, int y)		//ışığın konumunu hareket ettirmek için
{
	spinx = (spinx + (x - beginx)) % 360;	//farenin x koordinatını alıp spinx değişkenine modunu alarak eşitleme 
	beginx = x;
	glutPostRedisplay();
}

void myinit(void)
{
	glEnable(GL_LIGHTING);	//Işığı aktif etme
	glEnable(GL_LIGHT0);	//0'dan 7'ye kadar ışık var
	glTranslatef(0.0, 0.0, -3.2); 	//Neden -5? Çünkü sahneyi 5 birim arkaya alıp görmek için (kamera sabit hep)
	glDepthFunc(GL_LESS);	//Derinlik için aktif etme(başlatma)
	glEnable(GL_DEPTH_TEST);
}

void display(void)
{
	GLfloat position[] =
	{ 0.0, 0.0, 1.5, 1.0 };	//ışığın başlangıçtaki pozisyonu
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //tampon temizleme

	GLfloat DiffuseLight[] = { dlr, dlg, dlb }; //ışık için özel olarak belirlenmiş
	GLfloat AmbientLight[] = { alr, alg, alb };

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();		//Fazladan işlem yapmak için bu fonksiyonu kullandım

	glTranslatef(0.0, 0.0, yakuz);
	yakuz = 0;
	glPushMatrix();
	glRotated((GLdouble)spinx, 0.0, 1.0, 0.0); //ekranda ışığın konumunu belirten küçük karenin konumu (fare ile etkileşimli)

	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight); //göreceli olarak ışık değiştirme
	glLightfv(GL_LIGHT1, GL_AMBIENT, AmbientLight);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glTranslatef(0.0, 0.0, 1.5);   //küçük karenin merkeze olan z uzaklığını 1.5 ayarladım
	glDisable(GL_LIGHTING);		//ışığı deaktif etme
	glColor3f(dlr, dlg, dlb);  //küçük karenin rengi turuncu
	glutSolidCube(0.1);		//katı(solid) mı tel(wire) mı,   kalınlığı 0.1 
	glEnable(GL_LIGHTING);
	glPopMatrix();

	switch (obj) {
	case Simit:
		glPushMatrix();	//Cisimleri çizdirmek için PushMatrix ve pop matrixin arasında kullandım.
		glColor3f(1.0, 1.0, 1.0);   //Simitin rengi beyaz ama ışıktan dolayı değişecektir.
		glRotatef(TX, 1.0, 0.0, 0.);	//oluşan cismi x ekseninde döndürme için
		glRotatef(TY, 0.0, 1.0, 0.); 	//oluşan cismi y ekseninde döndürme için
		glutSolidTorus(0.275, 0.85, 20, 20);	//Hazır simit çizdirme katı cisim
		glPopMatrix();
		break;
	case Caydanlik:
		glutSolidTeapot(1.0);			//Hazır çaydanlık çizdirme katı cisim (tel şeklinde yaptım)
		break;
	}

	glutSwapBuffers();		//ikili tampon arasında geçiş
}

void Reshape(int w, int h)	//yeniden şekillendirme pencere boyutu değişikliği sırasında
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);	//pencere boyutuna göre perspektif belirleme
	glMatrixMode(GL_MODELVIEW);
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key){
	case 'q': exit(0);	 	//q çıkış için	
		break;
	case 'w':  TX -= 5.0f;	//cismi yukarı döndürme
		break;
	case 's': TX += 5.0f;	//cismi aşağı döndürme
		break;
	case 'a':  TY -= 5.0f;	//cismi sola döndürme
		break;
	case 'd':   TY += 5.0f;	//cismi sağa döndürme
		break;
	case 'r':			//IŞIĞIN RENGİNİ KIRMIZI YAPMA
		dlr = 1.0;
		dlg = 0.0;
		dlb = 0.0;

		break;
	case 'g':   			//IŞIĞIN RENGİNİ yeşil YAPMA
		dlr = 0.0;
		dlg = 1.0;
		dlb = 0.0;
		break;
	case 'b': 			//IŞIĞIN RENGİNİ mavi YAPMA
		dlr = 0.0;
		dlg = 0.0;
		dlb = 1.0;
		break;
	case 'e':   			//e tuşuyla IŞIĞIN RENGİNİ BEYAZ YAPMA
		dlr = 1.0;
		dlg = 1.0;
		dlb = 1.0;
		break;
	case 't': 			//cismi yakınlaştırma
		yakuz += 0.2f;
		break;
	case 'y': 			// cismi uzaklaştırma
		yakuz -= 0.2f;
		break;
	default:
		break;
	}
	glutPostRedisplay();	//tekrardan gösterim fonksiyonunu güncelleme güncelleme

}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);  //ikili tampon
	glutInitWindowSize(500, 500);	//pencere boyutu
	glutCreateWindow(argv[0]);
	myinit();
	glutKeyboardFunc(Keyboard);	  //klavye
	glutMouseFunc(movelight);     //fare  ışık ve menü(sağ tık) için
	glutMotionFunc(motion);		  //ışıpın konumu için fonksiyon
	glutReshapeFunc(Reshape);
	glutDisplayFunc(display);	  //gösterim fonksiyonumuz
	glutCreateMenu(menu_select);	//menü oluşturma
	glutAddMenuEntry("Simit", Simit);
	glutAddMenuEntry("Caydanlik", Caydanlik);
	glutAddMenuEntry("Cikis", cikis);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutMainLoop();					//ana fonksiyonu sonsuz döngüye sokma
	return 0;
}
