//54207039H Daniel Domenech Moreno
//		g++ -Wall -g converter.o prac2.cc -o prac2 			COMPILAR
//		./prac2					EJECUTAR
//Daniel Domenech Moreno 54207039H
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>
using namespace std;
string cleanString(string);

enum Error{
  ERR_OPTION,
  ERR_INTENT,
  ERR_EXAMPLE,
  ERR_RESPONSE,
  ERR_FILE,
  ERR_THRESHOLD,
  ERR_SIMILARITY,
  ERR_ARGS
}; 
const int KSIZE=10;
const int KMAXTEXTS=15;
const int KMAXTEXTL=1000;
const string intent="Intent name: ";
const string threshold = "Enter threshold: ";
const string algorithm = "Enter algorithm: ";
const string filename = "Enter filename: ";
const string prconfirmar = "Confirm [Y/N]?:";
const string saveallintents = "Save all intents [Y/N]?: ";


void error(Error n){
  switch(n){
    case ERR_OPTION:
      cout << "ERROR: wrong menu option" << endl;
      break;
    case ERR_INTENT:
      cout << "ERROR: wrong intent name" << endl;
      break;
    case ERR_EXAMPLE:
      cout << "ERROR: wrong example id" << endl;
      break;
    case ERR_RESPONSE:
      cout << "Lo siento, pero no tengo respuesta para eso" << endl;
      break;
    case ERR_FILE:
      cout << "ERROR: cannot open file" << endl;
      break;
    case ERR_THRESHOLD:
      cout << "ERROR: the threshold value must be between 0 and 1" << endl;
      break;
    case ERR_SIMILARITY:
      cout << "ERROR: valid values are \"jc\" (Jaccard) and \"ng\" (n-grams)" << endl;
      break;
    case ERR_ARGS:
      cout << "ERROR: invalid arguments" << endl;
  }
}

const string greetings[KSIZE]={
  "Hola, ¿en qué puedo ayudarte?",
  "¿Qué puedo hacer hoy por ti?",
  "Oh, gran prócer de los bits, ¿cuál es tu deseo?",
  "Buenas",
  "¿Qué quieres?",
  "¿Otra vez necesitas ayuda?",
  "¡Hola! Soy Megabot 3000. ¿En qué puedo ayudarte?",
  "Bonito día para ser Megabot 3000",
  "Pregunta sin miedo",
  "Soy todo oídos"
};

struct Example{
int id;
string text;
vector<string> tokens;
};

struct Intent{
string name;
vector<Example> examples;
string response;
};

struct Chatbot{
int nextId;

float threshold;
char similarity[3];
vector<Intent> intents;
};

struct BinIntent{
char name[KMAXTEXTS];
unsigned numExamples;
char response[KMAXTEXTL];
};

struct BinChatbot{
float threshold;
char similarity[3];
unsigned numIntents;
};

int CuentaIntents(Chatbot megabot,string nombre,bool &repe)
{

	for(unsigned i = 0; i<megabot.intents.size();i++)
	{
		
		if(megabot.intents[i].name==nombre)
		{
			repe=true;
			return i;
		}
	}	
	return -1;
}

string CreaIntents(Chatbot &megabot,string linea)
{
	bool separador,repe=false;
	unsigned i;
	Intent tempIntent;
	
			tempIntent.name="";
			tempIntent.response="";
			i=0	;
			separador=true;
			i++;
			while(i<linea.length() && separador==true)
			{
				if(linea[i]!='#')
					tempIntent.name = tempIntent.name + linea[i];
				else
				separador=false;
				

				
				i++;
			}
			while(i<linea.length())
			{
				tempIntent.response = tempIntent.response+linea[i];
				i++;
			}
			
			CuentaIntents(megabot,tempIntent.name,repe);
			
			if(!repe)
			{
				megabot.intents.push_back(tempIntent);
			}
			else
				error(ERR_INTENT);
			
		return tempIntent.name;
		/*else
		{
			tempIntent.examples="";
			i=0;
			while(i<linea.length())						//Falta algún método para añadir los examples una vez tenga los intents
			{
				tempIntent.example = examples+linea[i];
				i++;
			}
			cout<<"Example: "<<tempIntent.examples<<endl;
		}*/
	
}

string Donlimpio (string palabra)
{
	
	string tempo="";
	
	
	
	for(unsigned i = 0; i<=palabra.length(); i++)
	{
	
	
	
		if(isupper(palabra[i]))
		{
				palabra[i]=tolower(palabra[i]);
			
		}
		
		if(isalnum(palabra[i]))
		{
				tempo=tempo+palabra[i];
			
		}
	
	
	
	}
	if(tempo[tempo.length()-1]=='s' || tempo[tempo.length()-1]=='s')
	{
	
		tempo.erase(tempo.length()-1);
	
	}

	return tempo;
}

void QuitaRepes(Example &example)
{
	for(unsigned i= 0; i<example.tokens.size();i++)
	{
		for(unsigned k= i; k<example.tokens.size();k++)
		{
			if(example.tokens[i]==example.tokens[k] && i!=k)
			{
				example.tokens.erase(example.tokens.begin()+i);
				k--;
			}
		}
	}
}

int Comparador(Example example1, Example example2)
{
	unsigned comun=0;
	
	QuitaRepes(example1);
	QuitaRepes(example2);
	
	//cout<<example1.text<<" "<<example2.text<<endl;
	
	for(unsigned i= 0; i<example1.tokens.size();i++)
	{
		
		
		for(unsigned k = 0 ; k<example2.tokens.size();k++)
		{
			//cout<<example1.tokens[i]<<" "<<example2.tokens[k]<<endl;
			if(example1.tokens[i]==example2.tokens[k])
			{ 
				//cout<<example1.tokens[i]<<" "<<example2.tokens[k]<<endl;
				comun++;
			}
			

			
		}
		
	}
	return comun;
	
}

float Jaccard(Example tempExample,Chatbot megabot)
{
	float jaccard=0,tempo;
	float nIntent=-1;
	
	for(float i = 0; i<megabot.intents.size();i++)
	{
		for(float h = 0; h<megabot.intents[i].examples.size();h++)
		{
			
			tempo=float(Comparador(megabot.intents[i].examples[h],tempExample))/float(megabot.intents[i].examples[h].tokens.size());
		//	cout<<float(Comparador(megabot.intents[i].examples[h],tempExample))<<"/"<<float(megabot.intents[i].examples[h].tokens.size())<<"="<<tempo<<endl;
			
			if(tempo>jaccard)
			{
				
				jaccard=tempo;
				if(jaccard>=megabot.threshold)
					nIntent=i;
				
			}
			
		}	
	}
	//cout<<jaccard<<endl;
	return nIntent;
}

vector <string> DiviGramas(string palabra)	//divide en trigramas una palabra y quita duplicados
{
	vector <string> gramas;
	
	char grama[3];		
	
	unsigned k=0;
		
	do
	{	
		for(unsigned i = 0 ; i<3 ; i++)
		{
			grama[i]=palabra[k+i];
			
		}	
			gramas.push_back(grama);
		
		k++;
	}while(palabra[k+2]!='\0');
	
	return gramas;
}

bool AntiDuplicados(vector <string> palabras, string adicionar)
{
	bool esta=false;
	
	for(unsigned i=0 ; i<palabras.size() ; i++)
	{
		if(strcmp(adicionar.c_str(),palabras[i].c_str())==0)
			esta=true;
	}
	
	return esta;
	
}

float Ngramas(Example tempExample,Chatbot megabot)
{
	vector <string> gramas;
	vector <string> tempgramas;
	
	float numIntent=-1,tempo,valor=-1;
	
	Example pregunta;
	Example respuesta;

	
	for(unsigned i = 0 ; i<tempExample.tokens.size() ; i++)		//Creo el example con la pregunta
	{
		tempgramas=DiviGramas(tempExample.tokens[i]);
		
		for(unsigned k=0;k<tempgramas.size();k++)
		
			if(!AntiDuplicados(gramas,tempgramas[k]))
				gramas.push_back(tempgramas[k]);
	
	}
	pregunta.text=tempExample.text;
	pregunta.tokens=gramas;
	gramas.clear();
	
	for(unsigned i = 0 ; i<megabot.intents.size() ; i++)
	{
		for(unsigned k = 0 ; k<megabot.intents[i].examples.size();k++)
		{
			for(unsigned j = 0; j<megabot.intents[i].examples[k].tokens.size();j++)			//recorro todos los examples junto a sus tokens y creo un example temporal con el que comparar los gramas
			{
				tempgramas=DiviGramas(megabot.intents[i].examples[k].tokens[j]);
				
				for(unsigned h = 0; h<tempgramas.size();h++)
				{
					if(!AntiDuplicados(gramas,tempgramas[h]))
						gramas.push_back(tempgramas[h]);
						
					
				}
				
				
			}
			respuesta.text=megabot.intents[i].examples[k].text;		//Guardo una respuesta y llamo a compararla con la pregunta
			respuesta.tokens=gramas;
			gramas.clear();
			
			//cout<<float(Comparador(pregunta,respuesta))<<"/"<<float(respuesta.tokens.size())<<"=";
			//cout<<float(Comparador(pregunta,respuesta))/float(respuesta.tokens.size())<<endl;
			
			tempo=float(Comparador(pregunta,respuesta))/float(respuesta.tokens.size()+1);
			
			if(tempo>megabot.threshold && tempo>valor)
			{
				
				valor=tempo;
				numIntent=i;
			}
			
			respuesta.text="";
			respuesta.tokens.clear();
		}
	}
			
		
	return numIntent;
}

void showMainMenu(){
  cout << "1- Train" << endl
       << "2- Test" << endl
       << "3- Report" << endl
       << "4- Configure"<<endl
       << "5- Import data"<<endl	
       << "6- Export data"<<endl
	   << "7- Load chatbot"<<endl
	   << "8- Save chatbot"<<endl
	   << "q- Quit" << endl
       << "Option: ";
}

void showTrainMenu(){
	
  cout << "1- Add intent" << endl
       << "2- Delete intent" << endl
       << "3- Add example" << endl
       << "4- Delete example" << endl
       << "5- Add response" << endl
       << "b- Back to main menu" << endl
       << "Option: ";
}

void addIntent(Chatbot &megabot){
	string tempo;
	Intent Auxiliar;
	cout<<intent;
	getline(cin,tempo);
	bool repe=false;
	CuentaIntents(megabot,tempo,repe);
	if(repe)
		error(ERR_INTENT);
	else
	{
		Auxiliar.name=tempo;
		megabot.intents.push_back(Auxiliar);
	}
	}

void CreaExample(Chatbot &megabot,string palabra,string nombreintent)
{
		bool repe=false;
		unsigned ntokens=0,pos=CuentaIntents(megabot,nombreintent,repe);
		string tokensuma="";
		Example tempExample;
		tempExample.text=palabra;
			
		for(unsigned i = 0; i<palabra.length();i++)	// suma carácteres hasta formar una palabra (encontrar espacios)
		{

					
				if(palabra[i]!=' ')
					tokensuma=tokensuma+palabra[i];
					
				if((palabra[i]==' ' && palabra[i+1]!=' ') || (palabra[i]!=' ' && palabra[i+1]=='\0'))
				{	
					tokensuma=cleanString(tokensuma);				//quito eñes y cosas que molestan en los tokens (mayus . _ ?...)			
					tokensuma=Donlimpio(tokensuma);
					if(tokensuma.length()>=1)
					{
						ntokens++;
						tempExample.tokens.push_back(tokensuma);		
						tokensuma="";	
					}
				}
				
				
				
		}
		if(ntokens>=1)
		{
			tempExample.id=megabot.nextId;		
			megabot.nextId++;		
			megabot.intents[pos].examples.push_back(tempExample);
		}
	
}

void deleteIntent(Chatbot &megabot){
	string tempo;
	cout<<intent;
	getline(cin,tempo);
	char confirmar;
	bool repe=false;
	unsigned posi=CuentaIntents(megabot,tempo,repe);
	
	if(repe)
	{
		do
		{
			cout<<prconfirmar;cin>>confirmar;
		}while(confirmar!='N' && confirmar!='Y' && confirmar!='y' && confirmar!='n');
		
		if(confirmar=='Y' || confirmar=='y')
			megabot.intents.erase(megabot.intents.begin()+posi);
		}
	else
		error(ERR_INTENT);
	}
	
void addExample(Chatbot &megabot){
	
	string nombreintent,tokensuma="";
	bool repe=false;
	Example tempExample;
	
	cout<<intent;
	getline(cin,nombreintent);
	CuentaIntents(megabot,nombreintent,repe);
			
	if(!repe)

			error(ERR_INTENT);	
	else{
		
			while(tempExample.text!="q")			//para salir de los examples coloca "q"
			{	
				tokensuma="";	
				tempExample.text="";
				tempExample.tokens.clear();

			
				cout<<"New example: ";
				getline(cin,tempExample.text);
			

			
				if(tempExample.text!="q" && tempExample.text!="")
				{
					CreaExample(megabot,tempExample.text,nombreintent);	
			
				}
			}
		
		}
}

void deleteExample(Chatbot &megabot){
	
	int idexample;
	bool erroresta=true;


	cout<<"Example id:";
	cin>>idexample;
	
	for(unsigned i = 0; i<megabot.intents.size();i++)
	{
		for(unsigned k = 0; k<megabot.intents[i].examples.size();k++)
		{
			if(idexample==megabot.intents[i].examples[k].id)
			{		
					megabot.intents[i].examples.erase(megabot.intents[i].examples.begin()+k);
					erroresta=false;
			}
		}
	}
	
	if(erroresta==true)
		error(ERR_EXAMPLE);
}

void addResponse(Chatbot &megabot)
{
	string tempo;
	bool repe=false;
	cout<<intent;
	getline(cin,tempo);
	unsigned pos=CuentaIntents(megabot,tempo,repe); 		
	if(!repe)
			error(ERR_INTENT);	
	else
	{
		cout<<"New response: ";
		getline(cin,tempo);
		megabot.intents[pos].response=tempo;
	}
}

void Configure(Chatbot &megabot) 	//DONETES
{
	float thresh;
	char algort[3],jacc[3],ngrams[3];
	
	strcpy(jacc,"jc");
	strcpy(ngrams,"ng");
	
	cout<<threshold; cin>>thresh;
	if(thresh<0 || thresh>1)
		error(ERR_THRESHOLD);
	else
		megabot.threshold=thresh;
		
	cout<<algorithm; cin>>algort;
	
	if(strcmp(algort,jacc) && strcmp(algort,ngrams))
		error(ERR_SIMILARITY);
	else
		strcpy(megabot.similarity,algort);
}

void ImportaArchivos(ifstream &fimportar,Chatbot &megabot)
{
	string fname,linea="",nombreintent,textoexample;
	
	while(getline(fimportar,linea))
	{
		textoexample=linea;
		
		if(linea[0]=='#')
			nombreintent=CreaIntents(megabot,linea);
		else
			CreaExample(megabot,linea,nombreintent);
	}
}

void ImportData(Chatbot &megabot)	//DONETES
{
	string fname,linea="",nombreintent,textoexample;

	ifstream fimportar;

	
	cout<<filename;
	getline(cin,fname);
	fimportar.open(fname);
	
	if(!fimportar.is_open())
	{
		error(ERR_FILE);
	}
	else
	{
		ImportaArchivos(fimportar,megabot);
	}
	
	
	fimportar.close();
}

void GuardarIntent(Chatbot &megabot,string nombrefichero,int numintent)
{
	fstream fichero(nombrefichero,ios::app);
	
		fichero<<"#"<<megabot.intents[numintent].name<<"#"<<megabot.intents[numintent].response<<endl;
		
		for(unsigned i=0;i<megabot.intents[numintent].examples.size();i++)
		{	
			fichero<<megabot.intents[numintent].examples[i].text<<endl;
		}
		
		fichero.close();
	

	
}

void ExportData(Chatbot &megabot)	//DONETES
{
	char respuesta;
	string nombre,archivo;
	bool repe=false;
	int pos;
	
	do
	{
		cout<<saveallintents;cin>>respuesta;
	}while(respuesta!='n' && respuesta!='y' && respuesta!='N' && respuesta!='Y');
	
	if(respuesta=='n' || respuesta=='N')
	{
		cout<<intent;cin>>nombre;
		pos=CuentaIntents(megabot,nombre,repe);
		cout<<filename;
		cin.ignore();
		getline(cin,archivo);
		GuardarIntent(megabot,archivo,pos);
	}
		
	else
	{
		cout<<filename;
		cin.ignore();
		getline(cin,archivo);
		ofstream fichero(archivo);							//Borrar previo fichero
		if(fichero.is_open())
		{
			for(unsigned i = 0; i<megabot.intents.size() ;i++)
				{
					GuardarIntent(megabot,archivo,i);
	
				}
		}
		else
			error(ERR_FILE);
	}
	
	
}

void BorrarChatbot(Chatbot &megabot)
{		
	megabot.nextId=1;
	megabot.intents.clear();
	
}
void Binariobot(string nombrearchivo,ifstream &fichero,Chatbot &megabot)
{
	BinChatbot tempBinbot;
	BinIntent tempIntention;
	Intent tempintent;
	char tempexample[KMAXTEXTL];
	bool repe;

	fichero.read((char*)&tempBinbot,sizeof(BinChatbot));
	strcpy(megabot.similarity,tempBinbot.similarity);
	megabot.threshold=tempBinbot.threshold;
	
	for(unsigned i = 0 ; i<tempBinbot.numIntents; i++)
	{
		fichero.read((char*)&tempIntention,sizeof(BinIntent));

		tempintent.name="";
		tempintent.response="";
		tempintent.examples.clear();
		repe=false;
		CuentaIntents(megabot,tempIntention.name,repe);
		if(!repe)
		{
			tempintent.name=tempIntention.name;
			tempintent.response=tempIntention.response;
			
			megabot.intents.push_back(tempintent);
			
			for(unsigned k = 0; k<tempIntention.numExamples;k++)
			{
				
				fichero.read((char*)&tempexample,sizeof(tempexample));	
				CreaExample(megabot,tempexample,tempIntention.name);	

			}

		}
		else
			error(ERR_INTENT);
	}
		
	
	fichero.close();

}
void LoadChatbot(Chatbot &megabot) //DONETES
{
	char confirmar;
	string nombrearchivo;
	ifstream fichero;
	
	cout<<filename;getline(cin,nombrearchivo);
	fichero.open(nombrearchivo,ios::binary);
	
	if(!fichero.is_open())
	{
		error(ERR_FILE);
	}
	else
	{
		
		do{
			cout<<prconfirmar;cin>>confirmar;
			confirmar=tolower(confirmar);
		}while(confirmar!='n' && confirmar!='y');

		if(confirmar=='y')
		{
			BorrarChatbot(megabot);
			Binariobot(nombrearchivo,fichero,megabot);
		}
	}
}

void SaveChatbot(Chatbot megabot)
{
	ofstream fichero;
	string nombrearchivo;
	BinChatbot tempchatbot;
	BinIntent tempintent;
	char tempexample[KMAXTEXTL];
	
	cout<<filename;cin>>nombrearchivo;
	fichero.open(nombrearchivo);
	
	tempchatbot.threshold=megabot.threshold;
	strcpy(tempchatbot.similarity,megabot.similarity);		//copio el chatbot
	tempchatbot.numIntents=megabot.intents.size();
	
	if(fichero.is_open())
	{
		fichero.write((char*)&tempchatbot,sizeof(BinChatbot));
		for(unsigned i = 0; i<tempchatbot.numIntents;i++)
		{
			
			while(megabot.intents[i].name.length()>14)
			{
				megabot.intents[i].name.erase(megabot.intents[i].name.end()-1,megabot.intents[i].name.end());
			}
			strcpy(tempintent.name,megabot.intents[i].name.c_str());
			strcpy(tempintent.response,megabot.intents[i].response.c_str());	//copio el intent
			tempintent.numExamples=megabot.intents[i].examples.size();
			fichero.write((char*)&tempintent,sizeof(BinIntent));
			
			for(unsigned k = 0; k<tempintent.numExamples; k++)
			{
				strcpy(tempexample,megabot.intents[i].examples[k].text.c_str());
				fichero.write((char*)&tempexample,sizeof(tempexample));			//copio los ejemplos
			}
		}
		
	}
	else
		error(ERR_FILE);
		
	fichero.close();
	
	
}

void train(Chatbot &megabot){
	char option;
	do{
		showTrainMenu();
		cin >> option;
		cin.get();
        
		switch (option){ 
		case '1':
			addIntent(megabot);
			break;
		case '2':
			deleteIntent(megabot);
			break;
		case '3':
			addExample(megabot);
			break;
		case '4':
			deleteExample(megabot);
			break;
		case '5':
			addResponse(megabot);
			break;
		case 'b':
			
			break;
		default:
			error(ERR_OPTION);
		
		}
		
	}while(option!='b');
      
}

void test(const Chatbot &megabot)
{
	int position = rand()%KSIZE,nIntent;
	string comando,palabra;
	Example tempExample;
	
		cout<<">>"<<greetings[position]<<endl;
		
	do
	{
		palabra="";
		tempExample.text="";
		tempExample.tokens.clear();
		cout<<"<< ";
		getline(cin,comando);
		tempExample.text=comando;
		if(comando!="q")
		{
			for(unsigned i = 0; i<comando.length();i++)	// suma carácteres hasta formar una palabra (encontrar espacios)
				{	
						if(comando[i]!=' ')
							palabra=palabra+comando[i];
						if((comando[i]==' ' && comando[i+1]!=' ') || (comando[i]!=' ' && comando[i+1]=='\0')) 
						{	
							palabra=cleanString(palabra);										//quito eñes y cosas que molestan en los tokens (mayus . _ ?...)			
							palabra=Donlimpio(palabra);
							tempExample.tokens.push_back(palabra);
							palabra="";
						}
					}
		if(string(megabot.similarity)=="jc")
			nIntent=Jaccard(tempExample,megabot);
		else if(string(megabot.similarity)=="ng")
			nIntent=Ngramas(tempExample,megabot);
		
		if(nIntent!=-1) 
			cout<<">> "<<megabot.intents[nIntent].response<<endl;
		else
			error(ERR_RESPONSE);
	}
		
	}while(comando!="q");
}
void report(const Chatbot &megabot){
	string similar;
	float nexamples=0,i;
	cout<<"Similarity: ";
	if(strcmp(megabot.similarity,"jc")==0)
		similar="Jaccard";
	else if(strcmp(megabot.similarity,"ng")==0)
		similar="N-grams";
	cout<<similar<<endl;
	cout<<"Threshold: ";
	cout<<megabot.threshold<<endl;

	for(i = 0; i<megabot.intents.size();i++)
	{
		
		cout<<"Intent: ";
		cout<<megabot.intents[i].name<<endl; 					//Mostrar los Intents
		cout<<"Response: "<<megabot.intents[i].response<<endl;
		nexamples=nexamples+megabot.intents[i].examples.size();
			
			for(unsigned k=0;k<megabot.intents[i].examples.size();k++)
				{
					
					cout<<"Example "<<megabot.intents[i].examples[k].id<<": ";
					cout<<megabot.intents[i].examples[k].text<<endl; 					//Mostrar los Examples
					cout<<"Tokens "<<megabot.intents[i].examples[k].id<<": ";//<<megabot.intents[i].examples[k].tokens.size();
					for(unsigned h = 0;h<megabot.intents[i].examples[k].tokens.size();h++)
						{
							cout<<"<"<<megabot.intents[i].examples[k].tokens[h]<<"> ";
						}
					
					cout<<endl; 			
				}
			
	}	
	cout<<"Total intents: "<<megabot.intents.size()<<endl;
	cout<<"Total examples: "<<nexamples<<endl;
	if(nexamples!=0 && megabot.intents.size()!=0)
		cout<<"Examples per intent: "<<nexamples/megabot.intents.size()<<endl;
	else
		cout<<"Examples per intent: "<<0<<endl;
	
	}

void Argumentos(int argc,char *argv[],Chatbot &megabot)
{
	for(int i = 1 ; i<argc ; i++)
	{
		if(strcmp(argv[i],"-i")==0)		// Importar Intenciones 
		{
			i++;
			ifstream fimportar(argv[i]);
			if(fimportar.is_open())
			{
				ImportaArchivos(fimportar,megabot);
			}
			else
				error(ERR_FILE);
			
			
		}
		else if(strcmp(argv[i],"-t")==0)
		{
			test(megabot);
		}
		else if(strcmp(argv[i],"-l")==0)
		{
			i++;
			ifstream fimportar(argv[i]);
			if(fimportar.is_open())
			{
				Binariobot(argv[i],fimportar,megabot);
			}
			else
				error(ERR_FILE);
		}
		else
			error(ERR_ARGS);
					
	}
	
}

int main(int argc,char *argv[]){
  Chatbot megabot;
  megabot.nextId=1;
  megabot.threshold=0.25;
  strcpy(megabot.similarity,"jc");
    
  srand(666);

	if(argc>1)
		Argumentos(argc,argv,megabot);
	
  char option;
  do{
    showMainMenu();
    cin >> option;
    cin.get();
        
    switch (option){ 
      case '1':
        train(megabot);
        break;
      case '2':
        test(megabot);
        break;
      case '3':
        report(megabot);
        break;
      case '4':
		Configure(megabot);
		break;
	  case '5':
		ImportData(megabot);
		break;
	  case '6':
		ExportData(megabot);
		break;
	  case '7':
		LoadChatbot(megabot);
		break;
	  case '8':
		SaveChatbot(megabot);
		break;
      case 'q':
        break;
      default:
        error(ERR_OPTION);
        
        
    }
  }while(option!='q');
    
  return 0;
}
