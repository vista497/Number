#include "numbernet.h"
#include "ui_numbernet.h"
#include <QTextStream>
#include <omp.h>

NumberNet::NumberNet(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NumberNet)
{
    ui->setupUi(this);
    setWindowTitle("Draw digit:)");
    QWidget* MyWidget = new QWidget(this);
    MyWidget->setLayout(ui->gridLayout);
    setCentralWidget(MyWidget);
    resize(500, 380);


    NW_config = ReadDataNetWork("C:\\Project421\\Ekaterina\\Config.txt");
    NW=new Neuron(NW_config, 1);

    NW->LoadNet();
    ui->lcdNumber->display("");

    QObject::connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(clearLCD()));
    QObject::connect(ui->pushButton_2, SIGNAL(clicked()), ui->widget, SLOT(clear()));
    QObject::connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(on_pushButton_3_clicked()));
    QObject::connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(StartGuess()));
}

NumberNet::~NumberNet()
{
    delete NW;
    delete ui;
}

void NumberNet::ReadTest(double* input, int input_n) {
    QFile mFile("C:\\Project421\\Ekaterina\\test.txt");
    mFile.open(QIODevice::ReadOnly);
    QTextStream fin(&mFile);
    if (mFile.isOpen()) {
        for (int j = 0; j < input_n; j++)
            fin >> input[j];
    }
    else QMessageBox::information(0, "Error", "Error reading the file");
    mFile.close();


}
void NumberNet::clearLCD() {
    ui->lcdNumber->display("");
}
void NumberNet::StartGuess() {
    double* input = new double[NW_config.size[0]];
    ReadTest(input, NW_config.size[0]);



    NW->SetInputs(input);
    double digit = NW->ForwardFeed();
    ui->lcdNumber->display(QString().setNum(digit));
}
data_Net NumberNet::ReadDataNetWork(string path) {
    data_Net data{};
    ifstream fin;
    fin.open(path);
    if (!fin.is_open()) {
        cout << "Error reading the file " << path << endl;
        throw runtime_error("rror reading the file\n");
    }
    else
        cout << path << " loading...\n";
    string tmp;
    int n;
    while (!fin.eof()) {
        fin >> tmp;
        if (tmp == "NetWork") {
            fin >> n;
            data.n = n;
            data.size = new int[n];
            for (int i = 0; i < n; i++) {
                fin >> data.size[i];
            }
        }
    }
    fin.close();
    return data;
}


//=================================================================
//обучение (нужна консоль)

//data_Net ReadDataNetWork(string path)
//{
//    data_Net data{};
//    ifstream fin;
//    fin.open(path);
//    if (!fin.is_open()) {
//        cout << "Error reading the file " << path << endl;
//        system("pause");
//    }
//    else
//        cout << path << " loading...\n";
//    string tmp;
//    int L;
//    while (!fin.eof()) {
//        fin >> tmp;
//        if (tmp == "NetWork") {
//            fin >> L;
//            data.layers = L;
//            data.size = new int[L];
//            for (int i = 0; i < L; i++) {
//                fin >> data.size[i];
//            }
//        }
//    }
//    fin.close();
//    return data;
//}
data_info* ReadData(string path, const data_Net& data_NW, int &examples) {
    data_info* data;
    ifstream fin;
    fin.open(path);
    if (!fin.is_open()) {
        cout << "Error reading the file " << path << endl;
        system("pause");
    }
    else
        cout << path << " loading... \n";
    string tmp;
    fin >> tmp;
    if (tmp == "Examples") {
        fin >> examples;
        cout << "Examples: " << examples << endl;
        data = new data_info[examples];
        for (int i = 0; i < examples; ++i)
            data[i].pixels = new double[data_NW.size[0]];


        for (int i = 0; i < examples; ++i) {
            fin >> data[i].digit;
            for (int j = 0; j < data_NW.size[0]; ++j) {
                fin >> data[i].pixels[j];
            }
        }
        fin.close();
        cout << "lib_MNIST loaded... \n";
        return data;
    }
    else {
        cout << "Error loading: " << path << endl;
        fin.close();
        return nullptr;
    }
}

void NumberNet::on_pushButton_3_clicked()
{
        data_Net NW_config;
        data_info* data;
        double ra = 0, right, predict, maxra = 0;  //ra- кол-во правильных ответов за эпоху, right-правильная цифра, predict-предсказаная цифра, maxra- макс кол-во правильных ответов
        int epoch = 0;  //эпоха
        int study=0;
        bool repeat = true;
        chrono::duration<double> time;

        //инициализация нейросети
        NW_config = ReadDataNetWork("C:\\Project421\\Ekaterina\\Config.txt");
        Neuron NW(NW_config, 1);
        NW.PrintConfig();

        while (repeat) {
            //вопрос нужно ли обучение?
            cout << "STUDY? (1/0)" << endl;
            cin >> study;

            if (study) {
                int examples; //кол-во обучающих премеров
                data = ReadData("C:\\Project421\\Ekaterina\\lib_MNIST_edit.txt", NW_config, examples);
                auto begin = chrono::steady_clock::now();

                while (ra / examples * 100 < 100) {
                    ra = 0;
                    auto t1 = chrono::steady_clock::now();


                    for (int i = 0; i < examples; ++i) {
                        NW.SetInputs(data[i].pixels);
                        right = data[i].digit;
                        predict = NW.ForwardFeed();
                        if (predict != right) {
                            NW.BackPropogation(right);
                            NW.WeightsUpdater(0.15 * exp(-epoch / 20.));
                        }
                        else
                            ra++;
                    }
                    auto t2 = chrono::steady_clock::now();
                    time = t2 - t1;
                    if (ra > maxra) maxra = ra;
                    cout << "ra: " << ra / examples * 100 << "\t" << "maxra: " << maxra / examples * 100 << "\t" << "epoch: " << epoch << "\tTIME: " << time.count() << endl;
                    epoch++;
                    if (epoch == 10)
                        break;
                }
                auto end = chrono::steady_clock::now();
                time = end - begin;
                cout << "TIME: " << time.count() / 60. << " min" << endl;
                NW.SaveNet();
            }
            else {
                NW.LoadNet();
            }
            cout << "Test? (1/0)\n";
            bool test_flag;
            cin >> test_flag;
            if (test_flag) {
                int ex_tests;
                data_info* data_test;
                data_test = ReadData("C:\\Project421\\Ekaterina\\lib_10k.txt", NW_config, ex_tests);
                ra = 0;
                for (int i = 0; i < ex_tests; ++i) {
                    NW.SetInputs(data_test[i].pixels);
                    predict = NW.ForwardFeed();
                    right = data_test[i].digit;
                    if (right == predict)
                        ra++;
                }
                cout << "RA: " << ra / ex_tests * 100 << endl;
            }
            cout << "Repeat? (1/0)\n";
            cin >> repeat;
        }

}


void NumberNet::on_comboBox_activated(int index)
{
    NW->BackPropogation(index);
    NW->WeightsUpdater(0.15 * exp(-14 / 20.));
    NW->SaveNet();
}

