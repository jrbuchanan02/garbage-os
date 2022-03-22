;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; 8086-OLD entry point.                                                      ;;
;;                                                                            ;;
;; Author: Joshua Buchanan (@jrbuchanan02)                                    ;;
;; Version: 1                                                                 ;;
;; Created: 2022-03-21                                                        ;;
;; Modified: 2022-03-21                                                       ;;
;; Copyright: 2022 (C) Joshua Buchanan; LGPL version 2.1                      ;;
;;                                                                            ;;
;; 16-bit x86 protected-mode operating system that has an identical ABI to    ;;
;; the 32 and 64-bit equivalents. This interoperability may allow full        ;;
;; "backwards" compatibility between the three architectures -- quite the     ;;
;; feat!                                                                      ;;
;;                                                                            ;;
;; Unfortunately, this backwards compatibility is not certain because there   ;;
;; may not be a way to run 16-bit protected mode software on 64-bit systems,  ;;
;; but, since the 64-bit systems bundle with the 32-bit mode systems, you     ;;
;; never know.                                                                ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

section .text
bits 16 ;; enforce 16-bit mode
cpu 286 ;; we're in protected mode, but have a 286.
global kmain16

kmain16:
    ;; entry point for our code. 

    ;; finished.
    cli
    hlt