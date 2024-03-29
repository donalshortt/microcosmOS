#!/bin/sh

ISO=kernel.iso
MEM=8192M

CC=gcc
SHARED_FLAGS="-fno-builtin -nostdinc -nostdlib -ffreestanding -Wall -Wextra -I. -MMD -mno-red-zone -no-pie -fno-pie -mcmodel=large -fno-stack-protector"
OBJECTS="util.o test.o pmm_test.o vmm_test.o pmm.o vmm.o mm.o front.o kernel.o boot.o vulns.o spectre.o flush_reload.o"
DFILES="util.d test.d pmm_test.d vmm_test.c pmm.d vmm.d mm.d front.d kernel.d boot.d vulns.d spectre.d flush_reload.d"

usage()
{
    echo "Builds and launches the kernel"
    echo "-h "
    echo "-d Build and launch debug mode in qemu (gdb)"
    echo "-q Build and launch normally in qemu"
    echo "-m Sets mem usage for qemu. Format: XXXXY"
	echo "-w Compile without running"
    echo "    X = Mem amount - Y = M/G, etc."
}

make_iso()
{
    mkdir -p iso/boot/grub                           
    cp microcosmOS/grub.cfg iso/boot/grub/           
    cp microcosmOS/kernel iso/boot/                  
    grub-mkrescue -o $ISO iso
}

compile_debug()
{
    cd microcosmOS

    $CC $SHARED_FLAGS -O0 -ggdb -c -o kernel.o core/kernel.c
    $CC $SHARED_FLAGS -O0 -ggdb -c -o boot.o core/boot.S
    $CC $SHARED_FLAGS -O0 -ggdb -c -o pmm.o mm/pmm/pmm.c
    $CC $SHARED_FLAGS -O0 -ggdb -c -o vmm.o mm/vmm/vmm.c
    $CC $SHARED_FLAGS -O0 -ggdb -c -o mm.o mm/mm.c
    $CC $SHARED_FLAGS -O0 -ggdb -c -o front.o front/front.c
    $CC $SHARED_FLAGS -O0 -ggdb -c -o test.o test/test.c
    $CC $SHARED_FLAGS -O0 -ggdb -c -o pmm_test.o test/pmm/pmm_test.c
	$CC $SHARED_FLAGS -O0 -ggdb -c -o vmm_test.o test/vmm/vmm_test.c
	$CC $SHARED_FLAGS -O0 -ggdb -c -o vulns.o vulns/vulns.c
	$CC $SHARED_FLAGS -O0 -ggdb -c -o spectre.o vulns/spectre/spectre.c
	$CC $SHARED_FLAGS -O0 -ggdb -c -o flush_reload.o vulns/flush_reload/flush_reload.c
	$CC $SHARED_FLAGS -O0 -ggdb -c -o util.o util/util.c
    
	$CC $SHARED_FLAGS -O0 -ggdb -z -W1,--build-id=none -T core/kernel.ld -o kernel $OBJECTS
    
    cd ../
}

compile()
{
    cd microcosmOS

    $CC $SHARED_FLAGS -O0 -c -o kernel.o core/kernel.c
    $CC $SHARED_FLAGS -O0 -c -o boot.o core/boot.S
    $CC $SHARED_FLAGS -O0 -c -o pmm.o mm/pmm/pmm.c
    $CC $SHARED_FLAGS -O0 -c -o vmm.o mm/vmm/vmm.c
    $CC $SHARED_FLAGS -O0 -c -o mm.o mm/mm.c
    $CC $SHARED_FLAGS -O0 -c -o front.o front/front.c
    $CC $SHARED_FLAGS -O0 -c -o test.o test/test.c
	$CC $SHARED_FLAGS -O0 -c -o pmm_test.o test/pmm/pmm_test.c
	$CC $SHARED_FLAGS -O0 -c -o vmm_test.o test/vmm/vmm_test.c
	$CC $SHARED_FLAGS -O0 -c -o vulns.o vulns/vulns.c
	$CC $SHARED_FLAGS -O0 -c -o spectre.o vulns/spectre/spectre.c
	$CC $SHARED_FLAGS -O0 -c -o flush_reload.o vulns/flush_reload/flush_reload.c
	$CC $SHARED_FLAGS -O0 -c -o util.o util/util.c
    
	$CC $SHARED_FLAGS -O0 -z -W1,--build-id=none -T core/kernel.ld -o kernel $OBJECTS
    
    cd ../
}

clean()
{
    rm -rf iso
    rm -rf $ISO

    cd microcosmOS
    find -name "*~" -delete
    rm -rf $DFILES $OBJECTS
    cd ../
}

move_to_build_and_remove_dfiles()
{ 
    mv microcosmOS/*.o build/
    rm microcosmOS/*.d 
}

debug()
{
    clean
    compile_debug
    make_iso

    move_to_build_and_remove_dfiles

    qemu-system-x86_64 -s -S -cdrom $ISO -serial stdio -m $MEM
}

qemu()
{
    clean
    compile
    make_iso

    move_to_build_and_remove_dfiles

    qemu-system-x86_64 -accel kvm -cdrom $ISO -serial stdio -m $MEM
}

compile_without_run()
{
	clean
	compile
	make_iso

	move_to_build_and_remove_dfiles
}

main()
{
    while getopts ":hmdqwc" options; do
        case "${options}" in
        h)
            usage
            exit 0
            ;;
        m)
            MEM=${OPTARG}
            ;;
        d)
            debug
            ;;
        q)
            qemu
            ;;
		w)
			compile_without_run
			;;
        c)
            clean
            ;;
        esac
    done
    shift "$((OPTIND - 1))"
}

main "${@}"

exit 0
