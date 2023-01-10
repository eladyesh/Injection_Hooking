using System;
using System.IO;
using System.Threading;

class BarberShop
{
    
    // Initializing variables
    private static readonly object _shopLock = new object();
    private static readonly object _cashierLock = new object();
    private static readonly int MAX_CAPACITY = 10;
    private static readonly int WAITING_ROOM_CAPACITY = 4;
    private static readonly int BARBER_SEATS_CAPACITY = 3;
    private static int waitingCustomers = 0;
    private static int barberSeatsOccupied = 0;
    private static bool cashierFree = true;
    private static string to_write = "";
    private static readonly object _fileLock = new object();
    public static void main()
    {
        // Main function of program --> 20 clients
        for (int i = 1; i <= 20; i++)
        {
            Thread t = new Thread(new ParameterizedThreadStart(Customer));
            t.Start(i);
        }
    }

    private static void Customer(object id)
    {
        lock (_shopLock)
        {
            int customerId = (int)id;


            if (customerId >= 11 && customerId <= 20)
            {
                while (waitingCustomers + barberSeatsOccupied == MAX_CAPACITY || waitingCustomers == WAITING_ROOM_CAPACITY)
                {
                    Console.WriteLine("Customer {0} is waiting outside the shop", id);
                    to_write = $"Customer {id} is waiting outside the shop";
                    WriteToFile(to_write);
                    to_write = "";
                    Monitor.Wait(_shopLock);
                }
            }

            // Capacity in the store has reached
            if (waitingCustomers + barberSeatsOccupied == MAX_CAPACITY)
            {
                Console.WriteLine("Customer {0} left the shop - capacity reached", id);
                to_write += $"Customer {id} left the shop - capacity reached";
                WriteToFile(to_write);
                to_write = "";
                return;
            }

            // 3 people seating at barber seat
            if (barberSeatsOccupied == BARBER_SEATS_CAPACITY || waitingCustomers < WAITING_ROOM_CAPACITY)
            {
                //while (waitingCustomers == WAITING_ROOM_CAPACITY)
                //{
                //    Console.WriteLine("Customer {0} is waiting outside the shop", id);
                //    to_write += $"Customer {id} is waiting outside the shop";
                //    WriteToFile(to_write);
                //    to_write = "";
                //    Monitor.Wait(_shopLock);
                //}

                waitingCustomers++;
                Console.WriteLine("Customer {0} entered the waiting room", id);
                Monitor.Wait(_shopLock);
                to_write = $"Customer {id} entered the waiting room";
                WriteToFile(to_write);
                to_write = "";
                waitingCustomers--;
            }

            barberSeatsOccupied++;
            Console.WriteLine("Customer {0} is getting a haircut", id);

            to_write = $"Customer {id} is getting a haircut";
            WriteToFile(to_write);
            to_write = "";

        }

        // Simulating haircut process 
        Thread.Sleep(1000);
        Console.WriteLine("Customer {0} finished getting a haircut", id);

        //to_write += $"Customer {id} is getting a haircut";
        //WriteToFile(to_write);
        //to_write = "";

        lock (_cashierLock)
        {
            if (!cashierFree)
            {
                Console.WriteLine("Customer {0} is waiting for cashier", id);
                to_write = $"Customer {id} is waiting for cashier";
                WriteToFile(to_write);
                to_write = "";
                Monitor.Wait(_cashierLock);
            }
            Console.WriteLine("Customer {0} is paying", id);
            to_write = $"Customer {id} is paying";
            WriteToFile(to_write);
            to_write = "";
            cashierFree = false;
        }

        // Simulating payment process
        Thread.Sleep(1000);
        Console.WriteLine("Customer {0} finished paying", id);

        to_write = $"Customer {id} finished paying";
        WriteToFile(to_write);
        to_write = "";

        lock (_cashierLock)
        {
            cashierFree = true;
            Monitor.Pulse(_cashierLock); // Releasing the cashier lock
        }

        // Checking shop lock
        lock (_shopLock)
        {
            barberSeatsOccupied--;
            if (waitingCustomers > 0)
            {
                Monitor.Pulse(_shopLock);
            }
            if (waitingCustomers + barberSeatsOccupied < MAX_CAPACITY && waitingCustomers < WAITING_ROOM_CAPACITY)
            {
                Monitor.PulseAll(_shopLock);
            }
        }
    }

    private static void WriteToFile(string text)
    {
        lock (_fileLock)
        {
            text += "\n";
            try
            {
                File.AppendAllText("Barber Day.txt", text);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }
    }

}

class Program
{
    public static void Main(string[] args)
    {
        BarberShop.main();
        Console.ReadKey();
        Console.WriteLine("\nDone !!!");
    }
}
