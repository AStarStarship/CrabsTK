#include <chrono>
#include <iostream>
using namespace std;


inline void test_putchar_vs_printf_vs_cout () {

    std::chrono::time_point<std::chrono::system_clock> start,
        end;
    start = std::chrono::system_clock::now ();
    for (int i = 0; i < 512 * 1024; ++i) 
        putchar ('.');
    end = std::chrono::system_clock::now ();

    std::chrono::duration<double> elapsed_seconds_putchar = end - start;

    start = std::chrono::system_clock::now ();
    for (int i = 0; i < 512 * 1024; ++i)
        printf (".");
    end = std::chrono::system_clock::now ();

    std::chrono::duration<double> elapsed_seconds_printf = end - start;

    start = std::chrono::system_clock::now ();
    for (int i = 0; i < 512 * 1024; ++i)
        std::cout << ".";
    end = std::chrono::system_clock::now ();

    std::chrono::duration<double> elapsed_seconds_cout = end - start;

    cout << "\n\nputchar vs printf vs cout benchmark\n\n"
        "512 * 1024 single char calls:\n"
        "putchar (char) calls: " <<
        elapsed_seconds_putchar.count () << "s\n" <<
        "printf (const char*) calls: " <<
        elapsed_seconds_printf.count () << "s\n" <<
        "cout << const char* calls: " <<
        elapsed_seconds_cout.count () << "s\n";

    start = chrono::system_clock::now ();
    for (int i = 0; i < 512 * 1024; ++i) {
        putchar ('.');
        putchar ('.');
        putchar ('.');
        putchar ('.');
        putchar ('.');
        putchar ('.');
        putchar ('.');
        putchar ('.');
        putchar ('.');
        putchar ('.');
    }
    end = chrono::system_clock::now ();

    chrono::duration<double> elapsed_seconds_putchar = end - start;

    start = chrono::system_clock::now ();
    for (int i = 0; i < 512 * 1024; ++i)
        printf ("..........");

    end = chrono::system_clock::now ();

    chrono::duration<double> elapsed_seconds_printf = end - start;

    start = chrono::system_clock::now ();
    for (int i = 0; i < 512 * 1024; ++i)
        cout << "..........";
    end = chrono::system_clock::now ();

    chrono::duration<double> elapsed_seconds_putchar = end - start;

    start = chrono::system_clock::now ();
    for (int i = 0; i < 512 * 1024; ++i)
        printf ("..........");
    end = chrono::system_clock::now ();

    chrono::duration<double> elapsed_seconds_printf = end - start;

    start = chrono::system_clock::now ();
    for (int i = 0; i < 512 * 1024; ++i)
        cout << "..........";
    end = chrono::system_clock::now ();

    cout << "\n512 * 1024 * 10 chars calls:\n"
        "putchar (char) calls: " <<
        elapsed_seconds_putchar.count () << "s\n" <<
        "printf (const char*) calls: " <<
        elapsed_seconds_printf.count () << "s\n" <<
        "cout << const char* calls: " <<
        elapsed_seconds_cout.count () << "s\n";

    start = chrono::system_clock::now ();
    for (int i = 0; i < 512 * 1024; ++i)
        printf ("..........");

    end = chrono::system_clock::now ();

    chrono::duration<double> elapsed_seconds_printf = end - start;

    start = chrono::system_clock::now ();
    for (int i = 0; i < 512 * 1024; ++i)
        cout << "..........";
    end = chrono::system_clock::now ();

    chrono::duration<double> elapsed_seconds_putchar = end - start;

    start = chrono::system_clock::now ();
    for (int i = 0; i < 512 * 1024; ++i)
        printf ("%i%i%f%f", GetRandom);
    end = chrono::system_clock::now ();

    chrono::duration<double> elapsed_seconds_printf = end - start;

    start = chrono::system_clock::now ();
    for (int i = 0; i < 512 * 1024; ++i)
        cout << "..........";
    end = chrono::system_clock::now ();

    cout << "\n512 * 1024 * 10 chars calls:\n"
        "putchar (char) calls: " <<
        elapsed_seconds_putchar.count () << "s\n" <<
        "printf (const char*) calls: " <<
        elapsed_seconds_printf.count () << "s\n" <<
        "cout << const char* calls: " <<
        elapsed_seconds_cout.count () << "s\n";
}