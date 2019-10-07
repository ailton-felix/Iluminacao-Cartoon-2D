################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../source/Canvas.cpp \
../source/Color.cpp \
../source/Controller.cpp \
../source/Curve.cpp \
../source/HierarchyRegions.cpp \
../source/Image.cpp \
../source/ImageException.cpp \
../source/LightingModel.cpp \
../source/MyDrawable.cpp \
../source/Normal.cpp \
../source/Point2D.cpp \
../source/Point3D.cpp \
../source/PointDirection.cpp \
../source/RegionProperties.cpp \
../source/Regions.cpp \
../source/Thinning.cpp \
../source/UtilityFunctions.cpp \
../source/Vectorization.cpp 

OBJS += \
./source/Canvas.o \
./source/Color.o \
./source/Controller.o \
./source/Curve.o \
./source/HierarchyRegions.o \
./source/Image.o \
./source/ImageException.o \
./source/LightingModel.o \
./source/MyDrawable.o \
./source/Normal.o \
./source/Point2D.o \
./source/Point3D.o \
./source/PointDirection.o \
./source/RegionProperties.o \
./source/Regions.o \
./source/Thinning.o \
./source/UtilityFunctions.o \
./source/Vectorization.o 

CPP_DEPS += \
./source/Canvas.d \
./source/Color.d \
./source/Controller.d \
./source/Curve.d \
./source/HierarchyRegions.d \
./source/Image.d \
./source/ImageException.d \
./source/LightingModel.d \
./source/MyDrawable.d \
./source/Normal.d \
./source/Point2D.d \
./source/Point3D.d \
./source/PointDirection.d \
./source/RegionProperties.d \
./source/Regions.d \
./source/Thinning.d \
./source/UtilityFunctions.d \
./source/Vectorization.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


