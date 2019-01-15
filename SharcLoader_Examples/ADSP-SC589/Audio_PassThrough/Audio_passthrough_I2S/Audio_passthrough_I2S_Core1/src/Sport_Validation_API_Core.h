/*****************************************************************************
 * Sport_Validation_API_Core0.h
 *****************************************************************************/
/* --------------------------------------------------------------------------------------------------------------------
 * Sport Valid
 */
#ifndef __SPORT_VALIDATION_API_CORE0_H__
#define __SPORT_VALIDATION_API_CORE0_H__



void Pin_Configurator(void); 

/*****************************************************************************
* Function name: Sport_mode_config
* Description: This function configures SPORT as either transmitter or slave
* Arguments:Values
* device ID: 0,1,2,3...
* Half sport ID: A B
* Transmitter :1 transmitter, 0 receiver
* Return value: None.
******************************************************************************/
 
 

#define Sport_mode_config(deviceID,hSportID,transmitter)\
	    *pREG_SPORT##deviceID##_CTL_##hSportID &= (0xffffffff ^(1<<BITP_SPORT_CTL_##hSportID##_SPTRAN));\
        *pREG_SPORT##deviceID##_CTL_##hSportID |=transmitter<<BITP_SPORT_CTL_##hSportID##_SPTRAN;


/*****************************************************************************
* Function name: Sport_dataConfig
* Description: This function configures data related functionality of 
* Arguments:Values
 *  device ID
 * Half Sport ID
 * Slength :3 -31
 * LSB first :1
 * datatype :0 Right Justify with '0'fill
 *          :1 Right justify with 'Sign' fill
 *          :2 U law companding
 *          :3 A law companding
 *
 * Rjust enable: 1
 * Opmode  :0 DSP Serial/Multichannel
 *         :1 I2S
 * pack_enable :1
 * Tranmitter finish interrupt enable: 1
* Return value: None.
******************************************************************************/






#define Sport_dataConfig(deviceID,hSportID,slength,LSBfirst,dataType,Rjust,opmode,packEnable,tranFinisInt)\
         *pREG_SPORT##deviceID##_CTL_##hSportID &= (0xffffffff ^((0xf<<BITP_SPORT_CTL_SLEN)|(0x1<<BITP_SPORT_CTL_LSBF)|(0x3 <<BITP_SPORT_CTL_DTYPE)|(0x1<<BITP_SPORT_CTL_RJUST)|(0x1<<BITP_SPORT_CTL_OPMODE)|(0x1<<BITP_SPORT_CTL_PACK)|(0x1<<BITP_SPORT_CTL_TFIEN)));\
         *pREG_SPORT##deviceID##_CTL_##hSportID |=(slength<<BITP_SPORT_CTL_SLEN)|(LSBfirst<<BITP_SPORT_CTL_LSBF)|(dataType <<BITP_SPORT_CTL_DTYPE)|(Rjust<<BITP_SPORT_CTL_RJUST)|(opmode<<BITP_SPORT_CTL_OPMODE)|(packEnable<<BITP_SPORT_CTL_PACK)|(tranFinisInt<<BITP_SPORT_CTL_TFIEN);






/*****************************************************************************
* Function name: Sport_mode_config
* Description: This function configures SPORT as either transmitter or slave
* Arguments:Values
 * device ID 0,1,2,3...
 * Half sport ID A B
 * secEnable 1 to enable secondary
 * Pri Enable 1 to enable primary
* Return value: None.
******************************************************************************/

 
 

#define Sport_enable(deviceID,hSportID,secEnable,priEnable)\
		  *pREG_SPORT##deviceID##_CTL_##hSportID &= (0xffffffff ^((0x1<<BITP_SPORT_CTL_SPENSEC)|(0x1 << BITP_SPORT_CTL_SPENPRI)));\
		  *pREG_SPORT##deviceID##_CTL_##hSportID |=(secEnable<<BITP_SPORT_CTL_SPENSEC)|(priEnable << BITP_SPORT_CTL_SPENPRI);






/*****************************************************************************
* Function name: Sport_clockConfig
* Description: This function configures clock related functionality of 
* Arguments:Values
 * device ID :0,1,2,3...
 * Half sport ID: A B
 * divider :put clock diver value Integer or hex
 * internal clock: 1
 * raising edge: 1
 * Gated clock : 1
* Return value: None.
******************************************************************************/



#define Sport_clockConfig(deviceID,hSportID,divider,iClock,iEdge,gClock)\
         *pREG_SPORT##deviceID##_CTL_##hSportID &= (0xffffffff ^ ((0x1<<BITP_SPORT_CTL_ICLK)|(0x1 << BITP_SPORT_CTL_CKRE)|(0x1<<BITP_SPORT_CTL_GCLKEN)));\
         *pREG_SPORT##deviceID##_CTL_##hSportID |= (iClock<<BITP_SPORT_CTL_ICLK)|(iEdge << BITP_SPORT_CTL_CKRE)|(gClock<<BITP_SPORT_CTL_GCLKEN);\
         *pREG_SPORT##deviceID##_DIV_##hSportID &= (0xffffffff ^ (0xffff<<BITP_SPORT_DIV_CLKDIV));\
         *pREG_SPORT##deviceID##_DIV_##hSportID |=(divider<<BITP_SPORT_DIV_CLKDIV);






/*****************************************************************************
* Function name: Sport_frameConfig
* Description: This function configures frame related functionality of 
* Arguments:Values
* device ID
 * Half Sport ID
 * Divider (SLEN ideally)
 * Internal Frame sync 1
 * Late frame sync 1
 * Active Low frame sync 1 (left channel first in I2S mode Right channel first in Left justified mode
 * Data independent frame sync 1
 * Edge detect framesync 1
* Return value: None.
******************************************************************************/




#define Sport_frameConfig(deviceID,hSportID,divider,iFS,lateFS,lowFS,dataIndFS,edgedetFS,FSR)\
		*pREG_SPORT##deviceID##_CTL_##hSportID &= (0xffffffff  ^ ((0x1<<BITP_SPORT_CTL_IFS)|(0x1<<BITP_SPORT_CTL_LAFS)|(0x1 << BITP_SPORT_CTL_LFS)|(0x1<<BITP_SPORT_CTL_DIFS)|(0x1<<BITP_SPORT_CTL_FSED)|(0x1 << BITP_SPORT_CTL_FSR)));\
		*pREG_SPORT##deviceID##_CTL_##hSportID |= (iFS<<BITP_SPORT_CTL_IFS)|(lateFS<<BITP_SPORT_CTL_LAFS)|(lowFS <<BITP_SPORT_CTL_LFS)|(dataIndFS<<BITP_SPORT_CTL_DIFS)|(edgedetFS<<BITP_SPORT_CTL_FSED)|(FSR << BITP_SPORT_CTL_FSR);\
        *pREG_SPORT##deviceID##_DIV_##hSportID &= (0xffffffff ^ (0xffff<<BITP_SPORT_DIV_FSDIV));\
        *pREG_SPORT##deviceID##_DIV_##hSportID |=(divider<<BITP_SPORT_DIV_FSDIV);


/*****************************************************************************
* Function name: Sport_MultichannelConfig
* Description: This function configures multichannel related functionality of 
* Arguments:Values
* device ID
 * Half Sport ID
 * winsize: window size 0-127
 * offset:Depends on window size
 * frameDelay
 * Active Low frame sync 1 (left channel first in I2S mode Right channel first in Left justified mode
 * Data independent frame sync 1
 * Edge detect framesync 1
* Return value: None.
******************************************************************************/

#define Sport_MultichannelConfig(deviceID,hSportID,winsize,offset,frameDelay,mPackenable,Menable)\
		*pREG_SPORT##deviceID##_MCTL_##hSportID &=(0xffffffff ^((0x7f<<BITP_SPORT_MCTL_WSIZE)|(0x3ff<<BITP_SPORT_MCTL_WOFFSET)|(0xf<<BITP_SPORT_MCTL_MFD)|(0x1<<BITP_SPORT_MCTL_MCPDE)|(0x1<<BITP_SPORT_MCTL_MCE)));\
		*pREG_SPORT##deviceID##_MCTL_##hSportID |=(winsize<<BITP_SPORT_MCTL_WSIZE)|(offset<<BITP_SPORT_MCTL_WOFFSET)|(frameDelay<<BITP_SPORT_MCTL_MFD)|(mPackenable<<BITP_SPORT_MCTL_MCPDE)|(Menable<<BITP_SPORT_MCTL_MCE);


#define Sport_Mulichannel_Select(deviceID,hSportID,cs0,cs1,cs2,cs3)\
		*pREG_SPORT##deviceID##_CS0_##hSportID =cs0;\
		*pREG_SPORT##deviceID##_CS1_##hSportID =cs1;\
		*pREG_SPORT##deviceID##_CS2_##hSportID =cs2;\
		*pREG_SPORT##deviceID##_CS3_##hSportID =cs3;

#define Sport_EnableStatusInterrupt(deviceID,hSportID,dataPri,dataSec,framesync)\
		*pREG_SPORT##deviceID##_ERR_##hSportID &=(0xffffffff ^((0x1<<BITP_SPORT_ERR_DERRPMSK)|(0x1<<BITP_SPORT_ERR_DERRSMSK)|(0x1 <<BITP_SPORT_ERR_FSERRMSK)));\
        *pREG_SPORT##deviceID##_ERR_##hSportID |=(dataPri<<BITP_SPORT_ERR_DERRPMSK)|(dataSec<<BITP_SPORT_ERR_DERRSMSK)|(framesync <<BITP_SPORT_ERR_FSERRMSK);


#define Sport_SPMux(deviceID,hSportID,clockmux,framesyncmux)\
		*pREG_SPORT##deviceID##_CTL2_##hSportID &=(0xffffffff ^((0x1<<BITP_SPORT_CTL2_CKMUXSEL)|(0x1<<BITP_SPORT_CTL2_FSMUXSEL)));\
		*pREG_SPORT##deviceID##_CTL2_##hSportID |=(clockmux << BITP_SPORT_CTL2_CKMUXSEL)|(framesyncmux <<BITP_SPORT_CTL2_FSMUXSEL);





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* DMA ID  DMA ID value 0 1 2 ..
 * mSIZE 0 1 2 3 4 5
 * pSIZE 0 1 2 3
 * tWait wait for trigger 1
 * Interrupt 0 No interrupt
 *           1 XCOUNT
 *           2 YCOUNT
 *           3 Peripheral Interrupt
 *
 * Generate_trigger 0 No tigger
 *                  1 XCOUNT
 *                  2 YCOUNT
 *                  3 Peripheral Interrupt
 *
 * Flow             0 Stop
 *  				1 Auto Buffer
 *  				4 Descriptor List
 *  				5 Descriptor Array
 *
 * Sync         Enable 1
 * NDSIZE       0,1,2,3,4,5,6
 * Write_access  1 for receive
 * perif    1 to forward interrupt to peripheral
 * two_D    1 to enable 2D DMA
 * Trigger_Overrun  1 to enable trigger_overrun error
 * Descopy     1 to enable storing descriptor
 *Currentadd  1 to use Current address 0 for start address
 *
 */



#define Sport_DMA_ChannelConfig(dmaID,mSize,pSize,Twait,Interrupt_enable,Generate_trigger,flow,syncEnable,NDsize,write_access,periF,Two_D,Trigger_overun,Descriptor_cpy,Curr_Address)\
		*pREG_DMA##dmaID##_CFG &= (0xffffffff ^((0x7 << BITP_DMA_CFG_MSIZE)|(0x7 << BITP_DMA_CFG_PSIZE)|(0x1 << BITP_DMA_CFG_TWAIT)|(0x1 << BITP_DMA_CFG_INT) | (0x1 << BITP_DMA_CFG_TRIG)|(0x7 << BITP_DMA_CFG_FLOW)|(0x1<<BITP_DMA_CFG_SYNC)|(0x7 << BITP_DMA_CFG_NDSIZE)|(0x1<<BITP_DMA_CFG_WNR)|(0x1<<BITP_DMA_CFG_PDRF)|(0x1 <<BITP_DMA_CFG_TWOD)|(0x1 <<BITP_DMA_CFG_TOVEN ) |(0x1 <<BITP_DMA_CFG_DESCIDCPY  |(0x1<<BITP_DMA_CFG_CADDR))));\
        *pREG_DMA##dmaID##_CFG |= (mSize << BITP_DMA_CFG_MSIZE)|( pSize<< BITP_DMA_CFG_PSIZE)|(Twait << BITP_DMA_CFG_TWAIT)|(Interrupt_enable << BITP_DMA_CFG_INT) | (Generate_trigger << BITP_DMA_CFG_TRIG)|(flow << BITP_DMA_CFG_FLOW)|(syncEnable<<BITP_DMA_CFG_SYNC)|(NDsize << BITP_DMA_CFG_NDSIZE)|(write_access<<BITP_DMA_CFG_WNR)|(periF<<BITP_DMA_CFG_PDRF)|(Two_D <<BITP_DMA_CFG_TWOD)|(Trigger_overun <<BITP_DMA_CFG_TOVEN ) |(Descriptor_cpy <<BITP_DMA_CFG_DESCIDCPY|(Curr_Address<<BITP_DMA_CFG_CADDR));
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////////
/* DMA ID  DMA ID value 0 1 2 ..
 * Xcount
 * Ycount
 * Xmodify
 * Ymodify
 */

#define Sport_DMA_Counter_Config(dmaID,xcount,ycount,xmodifier,ymodifer)\
		*pREG_DMA##dmaID##_XCNT=xcount;\
		*pREG_DMA##dmaID##_YCNT=ycount;\
		*pREG_DMA##dmaID##_XMOD=xmodifier;\
		*pREG_DMA##dmaID##_YMOD=ymodifer;
/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
/* DMA ID  DMA ID value 0 1 2 ..
 * 1 for enable
 */
#define Sport_DMA_enable(dmaID,enable)\
         	*pREG_DMA##dmaID##_CFG &=(0xffffffff ^ (0x1<<BITP_DMA_CFG_EN));\
         	*pREG_DMA##dmaID##_CFG |=BITM_DMA_CFG_EN;
			
			
#define Sport_RJ_DelayConfig(deviceID,hSportID,delay)\
		*pREG_SPORT##deviceID##_MCTL_##hSportID &=(0xffffffff ^(0x3f<<16));\
		*pREG_SPORT##deviceID##_MCTL_##hSportID |=(delay<<16);

#endif /* __SPORT_VALIDATION_API_CORE0_H__ */
