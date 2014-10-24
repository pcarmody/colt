################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/AssocArray.cpp \
../src/avl.cpp \
../src/colt++.cpp \
../src/colt_cross.cpp \
../src/colt_html.cpp \
../src/colt_out_vertical.cpp \
../src/colt_range.cpp \
../src/colt_range_list.cpp \
../src/colt_select.cpp \
../src/colt_skip_limit.cpp \
../src/colt_to_json.cpp \
../src/coltaggregagerow.cpp \
../src/coltaggregate.cpp \
../src/coltbase.cpp \
../src/coltbitmap.cpp \
../src/coltcomplement.cpp \
../src/coltcounter.cpp \
../src/coltcsv.cpp \
../src/coltcthru.cpp \
../src/colteach.cpp \
../src/coltif.cpp \
../src/coltisect.cpp \
../src/coltkeyspace.cpp \
../src/coltonchange.cpp \
../src/coltoperator.cpp \
../src/coltout.cpp \
../src/coltparser.cpp \
../src/coltpartition.cpp \
../src/coltsequentialcsv.cpp \
../src/coltshare.cpp \
../src/coltsort.cpp \
../src/coltsync.cpp \
../src/coltthread.cpp \
../src/coltthru.cpp \
../src/coltunion.cpp \
../src/concurrentqueue.cpp \
../src/mylinkedlist.cpp 

OBJS += \
./src/AssocArray.o \
./src/avl.o \
./src/colt++.o \
./src/colt_cross.o \
./src/colt_html.o \
./src/colt_out_vertical.o \
./src/colt_range.o \
./src/colt_range_list.o \
./src/colt_select.o \
./src/colt_skip_limit.o \
./src/colt_to_json.o \
./src/coltaggregagerow.o \
./src/coltaggregate.o \
./src/coltbase.o \
./src/coltbitmap.o \
./src/coltcomplement.o \
./src/coltcounter.o \
./src/coltcsv.o \
./src/coltcthru.o \
./src/colteach.o \
./src/coltif.o \
./src/coltisect.o \
./src/coltkeyspace.o \
./src/coltonchange.o \
./src/coltoperator.o \
./src/coltout.o \
./src/coltparser.o \
./src/coltpartition.o \
./src/coltsequentialcsv.o \
./src/coltshare.o \
./src/coltsort.o \
./src/coltsync.o \
./src/coltthread.o \
./src/coltthru.o \
./src/coltunion.o \
./src/concurrentqueue.o \
./src/mylinkedlist.o 

CPP_DEPS += \
./src/AssocArray.d \
./src/avl.d \
./src/colt++.d \
./src/colt_cross.d \
./src/colt_html.d \
./src/colt_out_vertical.d \
./src/colt_range.d \
./src/colt_range_list.d \
./src/colt_select.d \
./src/colt_skip_limit.d \
./src/colt_to_json.d \
./src/coltaggregagerow.d \
./src/coltaggregate.d \
./src/coltbase.d \
./src/coltbitmap.d \
./src/coltcomplement.d \
./src/coltcounter.d \
./src/coltcsv.d \
./src/coltcthru.d \
./src/colteach.d \
./src/coltif.d \
./src/coltisect.d \
./src/coltkeyspace.d \
./src/coltonchange.d \
./src/coltoperator.d \
./src/coltout.d \
./src/coltparser.d \
./src/coltpartition.d \
./src/coltsequentialcsv.d \
./src/coltshare.d \
./src/coltsort.d \
./src/coltsync.d \
./src/coltthread.d \
./src/coltthru.d \
./src/coltunion.d \
./src/concurrentqueue.d \
./src/mylinkedlist.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


