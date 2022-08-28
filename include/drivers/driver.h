#ifndef __CPOS__DRIVERS__DRIVER_H
#define __CPOS__DRIVERS__DRIVER_H

namespace cpos 
{
    namespace drivers 
    {
        //驱动抽象
        class Driver {
        public:
            Driver();
            ~Driver();
            //启动
            virtual void Activate();
            //重置
            virtual int Reset();
            //关闭
            virtual void Deactivate();
        };

        class DriverManager {
        public:
            DriverManager();
            void AddDriver(Driver* );
            //启动所有驱动
            void ActivateAll();
        private:
            Driver* drivers[256];
            int numDrivers;
        };
    }
}


#endif 