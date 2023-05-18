pipeline {
    agent {
        node {
            label 'della_rk9481'
        }
    }

    stages {
        stage( ' Load git modules ' ){
            steps {
                echo ' Getting git submodules '
                sh 'git submodule init'
                sh 'git submodule update'
            }
        }
        stage (' GNU Unittests '){
            matrix {
                axes {
                    axis{
                        name 'HostSpace'
                        values 'SERIAL;-DKokkos_ENABLE_SERIAL=ON;-n 1', 'OPENMP;-DKokkos_ENABLE_OPENMP=ON;-n 10'
                    }
                    axis{
                        name 'DeviceSpace'
                        values 'NONE;-DKokkos_ENABLE_CUDA=OFF;--constraint=broadwell', 'CUDA_AMPERE80;-DKokkos_ENABLE_CUDA=ON -DKokkos_ARCH_AMPERE80=ON;--gres=gpu:1 --constraint=a100'
                    }
                }

                stages {
                    stage ('Build and Clean '){
                        environment {
                            CUDA_MODULE='cudatoolkit/11.7'
                            // CMAKE build flags
                            CMAKE_HOST_NAME = """${sh(
                                                    returnStdout: true,
                                                    script: 'cut -d";" -f1 <<<"${HostSpace}"'
                                                ).trim()}"""
                            CMAKE_HOST_FLAGS = """${sh(
                                                    returnStdout: true,
                                                    script: 'cut -d";" -f2 <<<"${HostSpace}"'
                                                ).trim()}"""
                            CMAKE_DEVICE_NAME = """${sh(
                                                    returnStdout: true,
                                                    script: 'cut -d";" -f1 <<<"${DeviceSpace}"'
                                                ).trim()}"""
                            CMAKE_DEVICE_FLAGS = """${sh(
                                                    returnStdout: true,
                                                    script: 'cut -d";" -f2 <<<"${DeviceSpace}"'
                                                ).trim()}"""
                            HOST_RUN_FLAGS = """${sh(
                                                    returnStdout: true,
                                                    script: 'cut -d";" -f3 <<<"${HostSpace}"'
                                                ).trim()}"""
                            DEVICE_RUN_FLAGS = """${sh(
                                                    returnStdout: true,
                                                    script: 'cut -d";" -f3 <<<"${DeviceSpace}"'
                                                ).trim()}"""
                        }
                        stages {
                            stage (' Build '){
                                steps {
                                    echo "Building ${CMAKE_HOST_FLAGS} ${CMAKE_DEVICE_FLAGS}"
                                    sh """
                                        module load ${CUDA_MODULE}
                                        cmake3 -S . -B build_GNU_${CMAKE_HOST_NAME}_${CMAKE_DEVICE_NAME} -DCMAKE_BUILD_TYPE=Release ${CMAKE_HOST_FLAGS} ${CMAKE_DEVICE_FLAGS}
                                        cmake3 --build build_GNU_${CMAKE_HOST_NAME}_${CMAKE_DEVICE_NAME}
                                    """
                                    echo ' Build completed '
                                }
                            }
                            stage (' Run Unittests '){
                                steps {
                                    echo " Running Unittests "
                                    sh """
                                        cd build_GNU_${CMAKE_HOST_NAME}_${CMAKE_DEVICE_NAME}/tests/unit-tests
                                        srun -N 1 -t 00:10:00 ${HOST_RUN_FLAGS} ${DEVICE_RUN_FLAGS} bash -c 'export OMP_PROC_BIND=spread; export OMP_THREADS=places; ctest'
                                    """
                                }
                            }
                            stage (' Clean '){
                                steps {
                                    echo ' Cleaning '
                                    sh "rm -rf build_GNU_${CMAKE_HOST_NAME}_${CMAKE_DEVICE_NAME}"
                                }
                            }
                        }
                    }
                }
            }
        }
        stage (' Intel Unittests '){
            matrix {
                axes {
                    axis{
                        name 'HostSpace'
                        values 'SERIAL;-DKokkos_ENABLE_SERIAL=ON;-n 1', 'OPENMP;-DKokkos_ENABLE_OPENMP=ON;-n 10'
                    }
                    axis{
                        name 'DeviceSpace'
                        values 'NONE;-DKokkos_ENABLE_CUDA=OFF;--constraint=broadwell'
                    }
                }

                stages {
                    stage ('Build and Clean '){
                        environment {
                            // CMAKE build flags
                            CMAKE_HOST_NAME = """${sh(
                                                    returnStdout: true,
                                                    script: 'cut -d";" -f1 <<<"${HostSpace}"'
                                                ).trim()}"""
                            CMAKE_HOST_FLAGS = """${sh(
                                                    returnStdout: true,
                                                    script: 'cut -d";" -f2 <<<"${HostSpace}"'
                                                ).trim()}"""
                            CMAKE_DEVICE_NAME = """${sh(
                                                    returnStdout: true,
                                                    script: 'cut -d";" -f1 <<<"${DeviceSpace}"'
                                                ).trim()}"""
                            CMAKE_DEVICE_FLAGS = """${sh(
                                                    returnStdout: true,
                                                    script: 'cut -d";" -f2 <<<"${DeviceSpace}"'
                                                ).trim()}"""
                            HOST_RUN_FLAGS = """${sh(
                                                    returnStdout: true,
                                                    script: 'cut -d";" -f3 <<<"${HostSpace}"'
                                                ).trim()}"""
                            DEVICE_RUN_FLAGS = """${sh(
                                                    returnStdout: true,
                                                    script: 'cut -d";" -f3 <<<"${DeviceSpace}"'
                                                ).trim()}"""
                        }
                        stages {
                            stage (' Build '){
                                steps {
                                    echo "Building ${CMAKE_HOST_FLAGS} ${CMAKE_DEVICE_FLAGS}"
                                    sh """
                                        module load intel/2022.2.0
                                        export CC=icx
                                        export CXX=icpx
                                        cmake3 -S . -B build_INTEL_${CMAKE_HOST_NAME}_${CMAKE_DEVICE_NAME} -DCMAKE_BUILD_TYPE=Release ${CMAKE_HOST_FLAGS} ${CMAKE_DEVICE_FLAGS}
                                        cmake3 --build build_INTEL_${CMAKE_HOST_NAME}_${CMAKE_DEVICE_NAME}
                                    """
                                    echo ' Build completed '
                                }
                            }
                            stage (' Run Unittests '){
                                steps {
                                    echo " Running Unittests "
                                    sh """
                                        cd build_INTEL_${CMAKE_HOST_NAME}_${CMAKE_DEVICE_NAME}/tests/unit-tests
                                        srun -N 1 -t 00:10:00 ${HOST_RUN_FLAGS} ${DEVICE_RUN_FLAGS} bash -c 'export OMP_PROC_BIND=spread; export OMP_THREADS=places; ctest'
                                    """
                                }
                            }
                            stage (' Clean '){
                                steps {
                                    echo ' Cleaning '
                                    sh "rm -rf build_INTEL_${CMAKE_HOST_NAME}_${CMAKE_DEVICE_NAME}"
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}