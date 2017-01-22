#ifndef   __uart_H__
#define   __uart_H__
extern uint8_t Recived_data;
extern void UART_init(uint32_t baudrate);
extern uint8_t UART_recive(void);
extern void UART_send_byte(uint8_t byte);
extern void UART_send(uint8_t *Buffer, uint32_t Length);
#endif