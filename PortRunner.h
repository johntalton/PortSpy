#ifndef PORT_RUNNER_H
#define PORT_RUNNER_H

struct PortItem{
   PortItem *next;
   bool multi;
   int sport;
   int eport;
   int curp; 
};

class PortRunner{
public:
   PortRunner(const char *str);
   virtual ~PortRunner();
   
   void Set(const char *str);
   
   int GetNextPort();
   int CountPorts();
   
   void PrintPorts();
private:
   void AddPort(int p);
   void AddPortRange(int s, int e);
   void RemoveList();
private:
   PortItem *head;
   PortItem *tail;
   
   PortItem *cur;
   int count;
};
#endif