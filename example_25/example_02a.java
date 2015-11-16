// file: ControlPanel.java
//

// import necessary java libraries
//
import javax.swing.*;
import javax.swing.border.*;
import java.lang.Number;
import javax.swing.event.*;
import java.awt.event.*;
import java.awt.*;
import java.util.*;


/**
 * ControlPanel controls the InputDrawingCanvas.  It allows for the user to
 * pick a waveform and set amplitude, xaxis and yaxis limits.
 *
 * hierarchy: JPanel->SubPanel->ControlPanel
 */
public class ControlPanel extends SubPanel implements ActionListener {
    
    // *********************************************************************
    //
    // declare global variables and components
    //
    // *********************************************************************

    // declare constants needed for axis parameters
    //
    static final double SIG_XPOS_INIT = (double)2.0;
    static final double SIG_XNEG_INIT = (double)-2.0;
    static final double SIG_YPOS_INIT = (double)2.0;
    static final double SIG_YNEG_INIT = (double)-2.0;
    static final double SIG_XTIC_INIT = (double)0.5;
    static final double SIG_YTIC_INIT = (double)0.5;

    static final double SYS_XPOS_INIT = (double)2.0;
    static final double SYS_XNEG_INIT = (double)-2.0;
    static final double SYS_YPOS_INIT = (double)2.0;
    static final double SYS_YNEG_INIT = (double)-2.0;
    static final double SYS_XTIC_INIT = (double)0.5;
    static final double SYS_YTIC_INIT = (double)0.5;

    static final double OUT_XPOS_INIT = (double)10.0;
    static final double OUT_XNEG_INIT = (double)-10.0;
    static final double OUT_YPOS_INIT = (double)2.0;
    static final double OUT_YNEG_INIT = (double)-2.0;
    static final double OUT_XTIC_INIT = (double)1.0;
    static final double OUT_YTIC_INIT = (double)0.5;

    static final double MAG_YPOS_INIT = (double)40.0;
    static final double MAG_YNEG_INIT = (double)-40.0;
    static final double MAG_XTIC_INIT = (double)0.25;
    static final double MAG_YTIC_INIT = (double)10.0;
    static final double MAG_FREQ_INIT = (double)2.0;

    // declare textfield initial parameters for the phase and magnitude plots
    //
    Double PI_MAX = new Double((double)3.14);
    Double PI_MIN = new Double((double)-3.14);
    Double INIT_FREQ_MAX = new Double((double)1.0);
    Double INIT_FREQ_MIN = new Double((double)-1.0);

    // declare the card layout identifier names
    //
    String signalParams = new String("Signal");
    String systemParams = new String("System");
    String outputParams = new String("Output");
    String magnitudeParams = new String("Magnitude");
    
    // declare strings to label the card in the card layout scheme
    //
    String signalTitle = new String("Signal:");
    String systemTitle = new String("System:");
    String outputTitle = new String("Output:");
    String magnitudeTitle = new String("Magnitude:");
    
    // declare the drawing panel objects 
    //
    SignalPanel signalPanel;
    SystemPanel systemPanel;
    OutputPanel outputPanel;
    OutputFrame magnitudePanel;

    // declare the card to be used in the card layout scheme
    //    
    JPanel cards;
    JPanel signalCard;
    JPanel systemCard;
    JPanel outputCard;
    JPanel magnitudeCard;
    
    // declare buttons to update the panel plots
    //
    JButton signalUpdateButton = new JButton("Update");
    JButton systemUpdateButton = new JButton("Update");
    JButton outputUpdateButton = new JButton("Update");
    JButton magnitudeUpdateButton = new JButton("Update");
    
    // declare components for the signal signal panel
    //
    Double SIG_X_TIC = new Double(SIG_XTIC_INIT);
    Double SIG_Y_TIC = new Double(SIG_YTIC_INIT);
    Double SIG_X_MAX = new Double(SIG_XPOS_INIT);
    Double SIG_X_MIN = new Double(SIG_XNEG_INIT);
    Double SIG_Y_MAX = new Double(SIG_YPOS_INIT);
    Double SIG_Y_MIN = new Double(SIG_YNEG_INIT);

    double signalXticDiv = SIG_X_TIC.doubleValue();
    double signalYticDiv = SIG_Y_TIC.doubleValue();
    double signalXposDiv = SIG_X_MAX.doubleValue();
    double signalXnegDiv = SIG_X_MIN.doubleValue();
    double signalYposDiv = SIG_Y_MAX.doubleValue();
    double signalYnegDiv = SIG_Y_MIN.doubleValue();
    
    JTextField signalXticDivision;
    JTextField signalYticDivision;
    JTextField signalXposLimit;
    JTextField signalXnegLimit;
    JTextField signalYposLimit;
    JTextField signalYnegLimit;
    
    JLabel inXticLabel = new JLabel("Time Tic");
    JLabel inYticLabel = new JLabel("Amp Tic");
    JLabel inXnegLabel = new JLabel("Time Min");
    JLabel inXposLabel = new JLabel("Time Max");
    JLabel inYnegLabel = new JLabel("Amp Min");
    JLabel inYposLabel = new JLabel("Amp Max");

    // declare components for the magnitude panel
    //
    Double MAG_X_TIC = new Double(MAG_XTIC_INIT);
    Double MAG_Y_TIC = new Double(MAG_YTIC_INIT);
    Double MAG_Y_MAX = new Double(MAG_YPOS_INIT);
    Double MAG_Y_MIN = new Double(MAG_YNEG_INIT);
    Double MAG_FREQ = new Double(MAG_FREQ_INIT);

    double magnitudeXticDiv = 0.0;
    double magnitudeYticDiv = 0.0;
    double magnitudeXposDiv = 0.0;
    double magnitudeXnegDiv = 0.0;
    double magnitudeYposDiv = 0.0;
    double magnitudeYnegDiv = 0.0;
    double magnitudefreqDiv = 0.0;
    
    JTextField magnitudeXticDivision;
    JTextField magnitudeYticDivision;
    JTextField magnitudeXposLimit;
    JTextField magnitudeXnegLimit;
    JTextField magnitudeYposLimit;
    JTextField magnitudeYnegLimit;
    JTextField magnitudefreqLimit;
    
    JLabel magXticLabel = new JLabel("Freq Tic");
    JLabel magYticLabel = new JLabel("Mag Tic");
    JLabel magXnegLabel = new JLabel("Freq Min");
    JLabel magXposLabel = new JLabel("Freq Max");
    JLabel magYnegLabel = new JLabel("Mag Min");
    JLabel magYposLabel = new JLabel("Mag Max");
    JLabel magfreqLabel = new JLabel("Sample Freq");

    // declare components for the system response panel
    //
    Double SYS_X_TIC = new Double(SYS_XTIC_INIT);
    Double SYS_Y_TIC = new Double(SYS_YTIC_INIT);
    Double SYS_X_MAX = new Double(SYS_XPOS_INIT);
    Double SYS_X_MIN = new Double(SYS_XNEG_INIT);
    Double SYS_Y_MAX = new Double(SYS_YPOS_INIT);
    Double SYS_Y_MIN = new Double(SYS_YNEG_INIT);

    double systemXticDiv = SYS_X_TIC.doubleValue();
    double systemYticDiv = SYS_Y_TIC.doubleValue();
    double systemXposDiv = SYS_X_MAX.doubleValue();
    double systemXnegDiv = SYS_X_MIN.doubleValue();
    double systemYposDiv = SYS_Y_MAX.doubleValue();
    double systemYnegDiv = SYS_Y_MIN.doubleValue();
    
    JTextField systemXticDivision;
    JTextField systemYticDivision;
    JTextField systemXposLimit;
    JTextField systemXnegLimit;
    JTextField systemYposLimit;
    JTextField systemYnegLimit;
    
    JLabel sysXticLabel = new JLabel("Time Tic");
    JLabel sysYticLabel = new JLabel("Amp Tic");
    JLabel sysXnegLabel = new JLabel("Time Min");
    JLabel sysXposLabel = new JLabel("Time Max");
    JLabel sysYnegLabel = new JLabel("Amp Min");
    JLabel sysYposLabel = new JLabel("Amp Max");

    // declare components for the output response panel
    //
    Double OUT_X_TIC = new Double(OUT_XTIC_INIT);
    Double OUT_Y_TIC = new Double(OUT_YTIC_INIT);
    Double OUT_X_MAX = new Double(OUT_XPOS_INIT);
    Double OUT_X_MIN = new Double(OUT_XNEG_INIT);
    Double OUT_Y_MAX = new Double(OUT_YPOS_INIT);
    Double OUT_Y_MIN = new Double(OUT_YNEG_INIT);

    double outputXticDiv = OUT_X_TIC.doubleValue();
    double outputYticDiv = OUT_Y_TIC.doubleValue();
    double outputXposDiv = OUT_X_MAX.doubleValue();
    double outputXnegDiv = OUT_X_MIN.doubleValue();
    double outputYposDiv = OUT_Y_MAX.doubleValue();
    double outputYnegDiv = OUT_Y_MIN.doubleValue();
    
    JTextField outputXticDivision;
    JTextField outputYticDivision;
    JTextField outputXposLimit;
    JTextField outputXnegLimit;
    JTextField outputYposLimit;
    JTextField outputYnegLimit;
    
    JLabel outXticLabel = new JLabel("Time Tic");
    JLabel outYticLabel = new JLabel("Amp Tic");
    JLabel outXnegLabel = new JLabel("Time Min");
    JLabel outXposLabel = new JLabel("Time Max");
    JLabel outYnegLabel = new JLabel("Amp Min");
    JLabel outYposLabel = new JLabel("Amp Max");
    
    // declare the default fonts
    //
    Font currentFont = getFont();
    Font newFont = new Font(currentFont.getName(), currentFont.getStyle(), 12);
    
    // *********************************************************************
    //
    // declare class constructors
    //
    // *********************************************************************

    /**
     * sets the minimum dimensions of the control panel
     */
    ControlPanel ()  {
	super();

	instantiate();

	// set the preferred size of the input canvas
	//
	setPreferredSize(new Dimension(220, 35));
    }
    
    /**
     *  uses the gui components just passed in to control the xaxis, 
     *  yaxis, xtic, ytic and other scales
     *
     *  @param out output panel gui object
     *  @param sig siganl panel gui object
     *  @param sys system panel gui object
     *  @param mag magnitude plot popup frame object
     *     
     */
    ControlPanel (OutputPanel out, SignalPanel sig,
		  SystemPanel sys, OutputFrame mag) {

	super();

	instantiate();

	signalPanel = sig;
	systemPanel = sys;
	outputPanel = out;
	magnitudePanel = mag;

	// set the preferred size of the input canvas
	//
	setPreferredSize(new Dimension(220, 35));
    }
    
    // *********************************************************************
    //
    // declare class methods
    //
    // *********************************************************************

    /**
     * instantiate components and add action listeners where necessary
     */
    public void instantiate() {
	
	
	// create a card layout to put the control and parameter items
	//
	cards = new JPanel();
	cards.setLayout(new CardLayout());
	
	// create a panel for the signal parameters
	//
	signalCard = new JPanel();
	
	// create a panel for the system parameters
	//
	systemCard = new JPanel();

	// create a panel for the system parameters
	//
	outputCard = new JPanel();

	// create a panel for the magnitude parameters
	//
	magnitudeCard = new JPanel();

	// set label default fonts and color for the labels
	//
	inXticLabel.setFont(newFont);
	inXticLabel.setForeground(Color.black);
	inYticLabel.setFont(newFont);
	inYticLabel.setForeground(Color.black);
	inXnegLabel.setFont(newFont);
	inXnegLabel.setForeground(Color.black);
	inXposLabel.setFont(newFont);
	inXposLabel.setForeground(Color.black);
	inYnegLabel.setFont(newFont);
	inYnegLabel.setForeground(Color.black);
	inYposLabel.setFont(newFont);
	inYposLabel.setForeground(Color.black);
	
	sysXticLabel.setFont(newFont);
	sysXticLabel.setForeground(Color.black);
	sysYticLabel.setFont(newFont);
	sysYticLabel.setForeground(Color.black);
	sysXnegLabel.setFont(newFont);
	sysXnegLabel.setForeground(Color.black);
	sysXposLabel.setFont(newFont);
	sysXposLabel.setForeground(Color.black);
	sysYnegLabel.setFont(newFont);
	sysYnegLabel.setForeground(Color.black);
	sysYposLabel.setFont(newFont);
	sysYposLabel.setForeground(Color.black);

	outXticLabel.setFont(newFont);
	outXticLabel.setForeground(Color.black);
	outYticLabel.setFont(newFont);
	outYticLabel.setForeground(Color.black);
	outXnegLabel.setFont(newFont);
	outXnegLabel.setForeground(Color.black);
	outXposLabel.setFont(newFont);
	outXposLabel.setForeground(Color.black);
	outYnegLabel.setFont(newFont);
	outYnegLabel.setForeground(Color.black);
	outYposLabel.setFont(newFont);
	outYposLabel.setForeground(Color.black);

	magXticLabel.setFont(newFont);
	magXticLabel.setForeground(Color.black);
	magfreqLabel.setFont(newFont);
	magfreqLabel.setForeground(Color.black);
	magYticLabel.setFont(newFont);
	magYticLabel.setForeground(Color.black);
	magXnegLabel.setFont(newFont);
	magXnegLabel.setForeground(Color.black);
	magXposLabel.setFont(newFont);
	magXposLabel.setForeground(Color.black);
	magYnegLabel.setFont(newFont);
	magYnegLabel.setForeground(Color.black);
	magYposLabel.setFont(newFont);
	magYposLabel.setForeground(Color.black);
	
	// set the default fonts for the update buttons
	//
	signalUpdateButton.setFont(newFont);
	systemUpdateButton.setFont(newFont);
	outputUpdateButton.setFont(newFont);
	magnitudeUpdateButton.setFont(newFont);
	
	// create the textfields for the signal panel
	//
	signalXticDivision = new JTextField(SIG_X_TIC.toString(), 5);
	signalYticDivision = new JTextField(SIG_Y_TIC.toString(), 5);
	signalXposLimit = new JTextField(SIG_X_MAX.toString(), 5);
	signalXnegLimit = new JTextField(SIG_X_MIN.toString(), 5);
	signalYposLimit = new JTextField(SIG_Y_MAX.toString(), 5);
	signalYnegLimit = new JTextField(SIG_Y_MIN.toString(), 5);
	
	// create the textfields for the system panel
	//
	systemXticDivision = new JTextField(SYS_X_TIC.toString(), 5);
	systemYticDivision = new JTextField(SYS_Y_TIC.toString(), 5);
	systemXposLimit = new JTextField(SYS_X_MAX.toString(), 5);
	systemXnegLimit = new JTextField(SYS_X_MIN.toString(), 5);
	systemYposLimit = new JTextField(SYS_Y_MAX.toString(), 5);
	systemYnegLimit = new JTextField(SYS_Y_MIN.toString(), 5);

	// create the textfields for the output panel
	//
	outputXticDivision = new JTextField(OUT_X_TIC.toString(), 5);
	outputYticDivision = new JTextField(OUT_Y_TIC.toString(), 5);
	outputXposLimit = new JTextField(OUT_X_MAX.toString(), 5);
	outputXnegLimit = new JTextField(OUT_X_MIN.toString(), 5);
	outputYposLimit = new JTextField(OUT_Y_MAX.toString(), 5);
	outputYnegLimit = new JTextField(OUT_Y_MIN.toString(), 5);

	// create the textfields for the magnitude panel
	//
	magnitudefreqLimit = new JTextField(MAG_FREQ.toString(), 5);
	magnitudeXticDivision = new JTextField(MAG_X_TIC.toString(), 5);
	magnitudeYticDivision = new JTextField(MAG_Y_TIC.toString(), 5);
	magnitudeXposLimit = new JTextField(INIT_FREQ_MAX.toString(), 5);
	magnitudeXnegLimit = new JTextField(INIT_FREQ_MIN.toString(), 5);
	magnitudeYposLimit = new JTextField(MAG_Y_MAX.toString(), 5);
	magnitudeYnegLimit = new JTextField(MAG_Y_MIN.toString(), 5);

	
	// set up the update for the signal panel and define the actions 
	// to take place when the button is clicked on
	//
	signalUpdateButton.addActionListener(new ActionListener() {
	
		public void actionPerformed(ActionEvent e) {
		
		// make sure the value is a valid Double
		//
		try {
		    signalXticDiv = Double.valueOf(signalXticDivision.
						   getText()).doubleValue();
		}
		catch (java.lang.NumberFormatException event) {
		    signalXticDiv =  SIG_X_TIC.doubleValue();
		    signalXticDivision.setText(SIG_X_TIC.toString());
		}
		try {
		    signalYticDiv = Double.valueOf(signalYticDivision.
						   getText()).doubleValue();
		}
		catch (java.lang.NumberFormatException event) {
		    signalYticDiv = SIG_Y_TIC.doubleValue();
		    signalYticDivision.setText(SIG_Y_TIC.toString());
		}
		try {
		    signalXposDiv = Double.valueOf(signalXposLimit.
						    getText()).doubleValue();
		}
		catch (java.lang.NumberFormatException event) {
		    signalXposDiv = SIG_X_MAX.doubleValue();
		    signalXposLimit.setText(SIG_X_MAX.toString());
		}
		try {
		    signalXnegDiv = Double.valueOf(signalXnegLimit.
						    getText()).doubleValue();
		}
		catch (java.lang.NumberFormatException event) {
		    signalXnegDiv = SIG_X_MIN.doubleValue();
		    signalXnegLimit.setText(SIG_X_MIN.toString());
		}	      
		try {
		    signalYposDiv = Double.valueOf(signalYposLimit.
						    getText()).doubleValue();
		}
		catch (java.lang.NumberFormatException event) {
		    signalYposDiv = SIG_Y_MAX.doubleValue();
		    signalYposLimit.setText(SIG_Y_MAX.toString());
		}
		try {
		    signalYnegDiv = Double.valueOf(signalYnegLimit.
						    getText()).doubleValue();
		}
		catch (java.lang.NumberFormatException event) {
		    signalYnegDiv = SIG_Y_MIN.doubleValue();
		    signalYnegLimit.setText(SIG_Y_MIN.toString());
		}
		
		// update the screen
		//
		repaintSignal();
	    }
	});
	    
	// set up the update for the system panel and define the actions 
	// to take place when the button is clicked on
	//
	systemUpdateButton.addActionListener(new ActionListener() {
	    public void actionPerformed(ActionEvent e) {
		
		// make sure the value is a valid double
		//
		try {
		    systemXticDiv = Double.valueOf(systemXticDivision.
						   getText()).doubleValue();
		}
		catch (java.lang.NumberFormatException event) {
		    systemXticDiv = SYS_X_TIC.doubleValue();
		    systemXticDivision.setText(SYS_X_TIC.toString());
		}
		try {
		    systemYticDiv = Double.valueOf(systemYticDivision.
						   getText()).doubleValue();
		}
		catch (java.lang.NumberFormatException event) {
		    systemYticDiv = SYS_Y_TIC.doubleValue();
		    systemYticDivision.setText(SYS_Y_TIC.toString());
		}
		try {
		    systemXposDiv = Double.valueOf(systemXposLimit.
						    getText()).doubleValue();
		}
		catch (java.lang.NumberFormatException event) {
		    systemXposDiv = SYS_X_MAX.doubleValue();
		    systemXposLimit.setText(SYS_X_MAX.toString());
		}
		try {
		    systemXnegDiv = Double.valueOf(systemXnegLimit.
						    getText()).doubleValue();
		}
		catch (java.lang.NumberFormatException event) {
		    systemXnegDiv = SYS_X_MIN.doubleValue();
		    systemXnegLimit.setText(SYS_X_MIN.toString());
		}	      
		try {
		    systemYposDiv = Double.valueOf(systemYposLimit.
						    getText()).doubleValue();
		}
		catch (java.lang.NumberFormatException event) {
		    systemYposDiv = SYS_Y_MAX.doubleValue();
		    systemYposLimit.setText(SYS_Y_MAX.toString());
		}
		try {
		    systemYnegDiv = Double.valueOf(systemYnegLimit.
						    getText()).doubleValue();
		}
		catch (java.lang.NumberFormatException event) {
		    systemYnegDiv = SYS_Y_MIN.doubleValue();
		    systemYnegLimit.setText(SYS_Y_MIN.toString());
		}
		
		// update the screen
		//
		repaintSystem();
	    }
	});

	// set up the update for the output panel and define the actions 
	// to take place when the button is clicked on
	//
	outputUpdateButton.addActionListener(new ActionListener() {
	    public void actionPerformed(ActionEvent e) {
		
		// make sure the value is a valid double
		//
		try {
		    outputXticDiv = Double.valueOf(outputXticDivision.
						   getText()).doubleValue();
		}
		catch (java.lang.NumberFormatException event) {
		    outputXticDiv = OUT_X_TIC.doubleValue();
		    outputXticDivision.setText(OUT_X_TIC.toString());
		}
		try {
		    outputYticDiv = Double.valueOf(outputYticDivision.
						   getText()).doubleValue();
		}
		catch (java.lang.NumberFormatException event) {
		    outputYticDiv = OUT_Y_TIC.doubleValue();
		    outputYticDivision.setText(OUT_Y_TIC.toString());
		}
		try {
		    outputXposDiv = Double.valueOf(outputXposLimit.
						    getText()).doubleValue();
		}
		catch (java.lang.NumberFormatException event) {
		    outputXposDiv = OUT_X_MAX.doubleValue();
		    outputXposLimit.setText(OUT_X_MAX.toString());
		}
		try {
		    outputXnegDiv = Double.valueOf(outputXnegLimit.
						    getText()).doubleValue();
		}
		catch (java.lang.NumberFormatException event) {
		    outputXnegDiv = OUT_X_MIN.doubleValue();
		    outputXnegLimit.setText(OUT_X_MIN.toString());
		}	      
		try {
		    outputYposDiv = Double.valueOf(outputYposLimit.
						    getText()).doubleValue();
		}
		catch (java.lang.NumberFormatException event) {
		    outputYposDiv = OUT_Y_MAX.doubleValue();
		    outputYposLimit.setText(OUT_Y_MAX.toString());
		}
		try {
		    outputYnegDiv = Double.valueOf(outputYnegLimit.
						    getText()).doubleValue();
		}
		catch (java.lang.NumberFormatException event) {
		    outputYnegDiv = OUT_Y_MIN.doubleValue();
		    outputYnegLimit.setText(OUT_Y_MIN.toString());
		}

		// repaint the output panel
		//
		repaintOutput();
	    }
	});

	// set up the update for the magnitude panel and define the actions 
	// to take place when the button is clicked on
	//
	magnitudeUpdateButton.addActionListener(new ActionListener() {
	    public void actionPerformed(ActionEvent e) {
		
		// make sure the value is a valid double
		//
		try {
		    magnitudefreqDiv = Double.valueOf(magnitudefreqLimit.
						   getText()).doubleValue();
		}
		catch (java.lang.NumberFormatException event) {
		    magnitudefreqDiv = MAG_FREQ.doubleValue();
		    magnitudefreqLimit.setText(MAG_FREQ.toString());
		}
		try {
		    magnitudeXticDiv = Double.valueOf(magnitudeXticDivision.
						   getText()).doubleValue();
		}
		catch (java.lang.NumberFormatException event) {
		    magnitudeXticDiv = MAG_X_TIC.doubleValue();
		    magnitudeXticDivision.setText(MAG_X_TIC.toString());
		}
		try {
		    magnitudeYticDiv = Double.valueOf(magnitudeYticDivision.
						   getText()).doubleValue();
		}
		catch (java.lang.NumberFormatException event) {
		    magnitudeYticDiv = MAG_Y_TIC.doubleValue();
		    magnitudeYticDivision.setText(MAG_Y_TIC.toString());
		}
		try {
		    magnitudeXposDiv = Double.valueOf(magnitudeXposLimit.
						    getText()).doubleValue();
		}
		catch (java.lang.NumberFormatException event) {
		    magnitudeXposDiv = INIT_FREQ_MAX.doubleValue();
		    magnitudeXposLimit.setText(INIT_FREQ_MAX.toString());
		}
		try {
		    magnitudeXnegDiv = Double.valueOf(magnitudeXnegLimit.
						    getText()).doubleValue();
		}
		catch (java.lang.NumberFormatException event) {
		    magnitudeXnegDiv = INIT_FREQ_MIN.doubleValue();
		    magnitudeXnegLimit.setText(INIT_FREQ_MIN.toString());
		}	      
		try {
		    magnitudeYposDiv = Double.valueOf(magnitudeYposLimit.
						    getText()).doubleValue();
		}
		catch (java.lang.NumberFormatException event) {
		    magnitudeYposDiv =  MAG_Y_MAX.doubleValue();
		    magnitudeYposLimit.setText(MAG_Y_MAX.toString());
		}
		try {
		    magnitudeYnegDiv = Double.valueOf(magnitudeYnegLimit.
						    getText()).doubleValue();
		}
		catch (java.lang.NumberFormatException event) {
		    magnitudeYnegDiv = MAG_Y_MIN.doubleValue();
		    magnitudeYnegLimit.setText(MAG_Y_MIN.toString());
		}
		
		// set the scale for the plot
		//
		magnitudePanel.data.sampleFrequency = magnitudefreqDiv;

		magnitudePanel.magnitude.xAxis.setXMax(magnitudeXposDiv);
		magnitudePanel.magnitude.xAxis.setXMin(magnitudeXnegDiv);
		magnitudePanel.magnitude.xAxis.setXTic(magnitudeXticDiv);
		magnitudePanel.magnitude.xAxis.resetXRange();

		magnitudePanel.magnitude.yAxis.setYMax(magnitudeYposDiv);
		magnitudePanel.magnitude.yAxis.setYMin(magnitudeYnegDiv);
		magnitudePanel.magnitude.yAxis.setYTic(magnitudeYticDiv);
		magnitudePanel.magnitude.yAxis.resetYRange();

		// update the scales to the appropriate values
		//
		magnitudePanel.magnitude.repaint();
	    }
	});
    }

    /**
     * resets the parameters in the signal textfield to the original default 
     * parameters
     */
    public void resetSignalParams() {

	// upadate the parameters in the textfield to the default ones
	//
	signalXticDiv = SIG_XTIC_INIT;
	signalXticDivision.setText(String.valueOf(SIG_XTIC_INIT));
	
	signalYticDiv = SIG_YTIC_INIT;
	signalYticDivision.setText(String.valueOf(SIG_YTIC_INIT));
	
	signalXposDiv = SIG_XPOS_INIT;
	signalXposLimit.setText(String.valueOf(SIG_XPOS_INIT));
	
	signalXnegDiv = SIG_XNEG_INIT;
	signalXnegLimit.setText(String.valueOf(SIG_XNEG_INIT));
	
	signalYposDiv = SIG_YPOS_INIT;
	signalYposLimit.setText(String.valueOf(SIG_YPOS_INIT));
	
	signalYnegDiv = SIG_YNEG_INIT;
	signalYnegLimit.setText(String.valueOf(SIG_YNEG_INIT));

	// repaint the signal panel
	//
	repaintSignal();
    }

    /**
     * resets the parameters in the system textfield to the original default
     * parameters
     */
    public void resetSystemParams() {

	// upadate the parameters in the textfield to the default ones
	//
	systemXticDiv = SYS_XTIC_INIT;
	systemXticDivision.setText(String.valueOf(SYS_XTIC_INIT));
	
	systemYticDiv = SYS_YTIC_INIT;
	systemYticDivision.setText(String.valueOf(SYS_YTIC_INIT));
	
	systemXposDiv = SYS_XPOS_INIT;
	systemXposLimit.setText(String.valueOf(SYS_XPOS_INIT));
	
	systemXnegDiv = SYS_XNEG_INIT;
	systemXnegLimit.setText(String.valueOf(SYS_XNEG_INIT));
	
	systemYposDiv = SYS_YPOS_INIT;
	systemYposLimit.setText(String.valueOf(SYS_YPOS_INIT));
	
	systemYnegDiv = SYS_YNEG_INIT;
	systemYnegLimit.setText(String.valueOf(SYS_YNEG_INIT));

	// repaint the system panel
	//
	repaintSystem();
    }

    /**
     * resets the parameters in the output textfield to the original default
     * parameters
     */
    public void resetOutputParams() {

	// upadate the parameters in the textfield to the default ones
	//
	outputXticDiv = OUT_XTIC_INIT;
	outputXticDivision.setText(String.valueOf(OUT_XTIC_INIT));
	
	outputYticDiv = OUT_YTIC_INIT;
	outputYticDivision.setText(String.valueOf(OUT_YTIC_INIT));
	
	outputXposDiv = OUT_XPOS_INIT;
	outputXposLimit.setText(String.valueOf(OUT_XPOS_INIT));
	
	outputXnegDiv = OUT_XNEG_INIT;
	outputXnegLimit.setText(String.valueOf(OUT_XNEG_INIT));
	
	outputYposDiv = OUT_YPOS_INIT;
	outputYposLimit.setText(String.valueOf(OUT_YPOS_INIT));
	
	outputYnegDiv = OUT_YNEG_INIT;
	outputYnegLimit.setText(String.valueOf(OUT_YNEG_INIT));

	// repaint the output panel
	//
	repaintOutput();
    }

    /**
     * method repaints the signal panel
     */
    public void repaintSignal() {

	// update the scales to the appropriate values
	//
	signalPanel.wavePanel.sigSetXDivision(signalXticDiv);
	signalPanel.wavePanel.sigSetYDivision(signalYticDiv);
	signalPanel.wavePanel.sigSetXPosLimit(signalXposDiv);
	signalPanel.wavePanel.sigSetYPosLimit(signalYposDiv);
	signalPanel.wavePanel.sigSetXNegLimit(signalXnegDiv);
	signalPanel.wavePanel.sigSetYNegLimit(signalYnegDiv);
	
	// get the last drawn waveform
	//
	int preset = signalPanel.wavePanel.presetWave;
	
	if(preset == 0) {
	    
	    // set the number of pixels to be drawn
	    //
	    signalPanel.wavePanel.currLimit =
		signalPanel.wavePanel.getNumPoints();
	}
	
	// update the screen
	//
	signalPanel.wavePanel.repaint();
    }

    /**
     * method repaints the system panel
     */
    public void repaintSystem() {

	// update the scales to the appropriate values
	//
	systemPanel.wavePanel.sysSetXDivision(systemXticDiv);
	systemPanel.wavePanel.sysSetYDivision(systemYticDiv);
	systemPanel.wavePanel.sysSetXPosLimit(systemXposDiv);
	systemPanel.wavePanel.sysSetYPosLimit(systemYposDiv);
	systemPanel.wavePanel.sysSetXNegLimit(systemXnegDiv);
	systemPanel.wavePanel.sysSetYNegLimit(systemYnegDiv);
	
	// get the last drawn waveform
	//
	int preset = systemPanel.wavePanel.presetWave;
	
	if(preset == 0) {
	    
	    // set the number of pixels to be drawn
	    //
	    systemPanel.wavePanel.currLimit =
		systemPanel.wavePanel.getNumPoints();
	}
	
	// update the screen
	//
	systemPanel.wavePanel.repaint();
    }

    /**
     * method repaints the output panel
     */
    public void repaintOutput() {

	// update the scales to the appropriate values
	//
	outputPanel.outDisp.outSetXDivision(outputXticDiv);
	outputPanel.outDisp.outSetYDivision(outputYticDiv);
	outputPanel.outDisp.outSetXPosLimit(outputXposDiv);
	outputPanel.outDisp.outSetYPosLimit(outputYposDiv);
	outputPanel.outDisp.outSetXNegLimit(outputXnegDiv);
	outputPanel.outDisp.outSetYNegLimit(outputYnegDiv);
	
	outputPanel.drawingArea.outSetXDivision(outputXticDiv);
	outputPanel.drawingArea.outSetYDivision(outputYticDiv);
	outputPanel.drawingArea.outSetXPosLimit(outputXposDiv);
	outputPanel.drawingArea.outSetYPosLimit(outputYposDiv);
	outputPanel.drawingArea.outSetXNegLimit(outputXnegDiv);
	outputPanel.drawingArea.outSetYNegLimit(outputYnegDiv);
	
	// repaint the output panel
	//
	outputPanel.outDisp.repaint();
	outputPanel.drawingArea.repaint();
    }
    
    /**    
     * positions individual controls on the Control Panel
     */
    public void addComponents() {
	
	// set the container layout for the components
	//
	GridBagLayout gridbag = new GridBagLayout();
	GridBagConstraints c = new GridBagConstraints();
	
	// define constraints for all components
	//
	c.weightx = 1.0;
	c.weighty = 1.0;
	c.gridheight = 1;
	c.anchor = GridBagConstraints.WEST;
	c.fill = GridBagConstraints.HORIZONTAL;
	
	// add components to the signal panel
	//
	c.gridx = 0;
	c.gridy = 0;
	c.gridwidth = 1;
	signalCard.setLayout(gridbag);
	gridbag.setConstraints(inXnegLabel, c);
	signalCard.add(inXnegLabel);
	
	c.gridx = 1;
	c.gridwidth = GridBagConstraints.REMAINDER;
	gridbag.setConstraints(signalXnegLimit, c);
	signalCard.add(signalXnegLimit);
	
	c.gridx = 0;
	c.gridy = 1;
	c.gridwidth = 1;
	gridbag.setConstraints(inXposLabel, c);
	signalCard.add(inXposLabel);
	
	c.gridx = 1;
	c.gridwidth = GridBagConstraints.REMAINDER;
	gridbag.setConstraints(signalXposLimit, c);
	signalCard.add(signalXposLimit);
	
	c.gridx = 0;
	c.gridy = 2;
	c.gridwidth = 1;
	gridbag.setConstraints(inXticLabel, c);
	signalCard.add(inXticLabel);
	
	c.gridx = 1;
	c.gridwidth = GridBagConstraints.REMAINDER;
	gridbag.setConstraints(signalXticDivision, c);
	signalCard.add(signalXticDivision);
	
	c.gridx = 0;
	c.gridy = 3;
	c.gridwidth = 1;
	gridbag.setConstraints(inYnegLabel, c);
	signalCard.add(inYnegLabel);
	
	c.gridx = 1;
	c.gridwidth = GridBagConstraints.REMAINDER;
	gridbag.setConstraints(signalYnegLimit, c);
	signalCard.add(signalYnegLimit);
	
	c.gridx = 0;
	c.gridy = 4;
	c.gridwidth = 1;
	gridbag.setConstraints(inYposLabel, c);
	signalCard.add(inYposLabel);
	
	c.gridx = 1;
	c.gridwidth = GridBagConstraints.REMAINDER;
	gridbag.setConstraints(signalYposLimit, c);
	signalCard.add(signalYposLimit);
	
	c.gridx = 0;
	c.gridy = 5;
	c.gridwidth = 1;
	gridbag.setConstraints(inYticLabel, c);
	signalCard.add(inYticLabel);
	
	c.gridx = 1;
	c.gridwidth = GridBagConstraints.REMAINDER;
	gridbag.setConstraints(signalYticDivision, c);
	signalCard.add(signalYticDivision);
	
	c.gridx = 0;
	c.gridy = 6;
	c.anchor = GridBagConstraints.CENTER;
	c.gridwidth = GridBagConstraints.REMAINDER;
	gridbag.setConstraints(signalUpdateButton, c);
	signalCard.add(signalUpdateButton);
	
	// add components to the system panel
	//
	c.gridx = 0;
	c.gridy = 0;
	c.gridwidth = 1;
	systemCard.setLayout(gridbag);
	gridbag.setConstraints(sysXnegLabel, c);
	systemCard.add(sysXnegLabel);
	
	c.gridx = 1;
	c.gridwidth = GridBagConstraints.REMAINDER;
	gridbag.setConstraints(systemXnegLimit, c);
	systemCard.add(systemXnegLimit);
	
	c.gridx = 0;
	c.gridy = 1;
	c.gridwidth = 1;
	gridbag.setConstraints(sysXposLabel, c);
	systemCard.add(sysXposLabel);
	
	c.gridx = 1;
	c.gridwidth = GridBagConstraints.REMAINDER;
	gridbag.setConstraints(systemXposLimit, c);
	systemCard.add(systemXposLimit);
	
	c.gridx = 0;
	c.gridy = 2;
	c.gridwidth = 1;
	gridbag.setConstraints(sysXticLabel, c);
	systemCard.add(sysXticLabel);
	
	c.gridx = 1;
	c.gridwidth = GridBagConstraints.REMAINDER;
	gridbag.setConstraints(systemXticDivision, c);
	systemCard.add(systemXticDivision);
	
	c.gridx = 0;
	c.gridy = 3;
	c.gridwidth = 1;
	gridbag.setConstraints(sysYnegLabel, c);
	systemCard.add(sysYnegLabel);
	
	c.gridx = 1;
	c.gridwidth = GridBagConstraints.REMAINDER;
	gridbag.setConstraints(systemYnegLimit, c);
	systemCard.add(systemYnegLimit);
	
	c.gridx = 0;
	c.gridy = 4;
	c.gridwidth = 1;
	gridbag.setConstraints(sysYposLabel, c);
	systemCard.add(sysYposLabel);
	
	c.gridx = 1;
	c.gridwidth = GridBagConstraints.REMAINDER;
	gridbag.setConstraints(systemYposLimit, c);
	systemCard.add(systemYposLimit);
	
	c.gridx = 0;
	c.gridy = 5;
	c.gridwidth = 1;
	gridbag.setConstraints(sysYticLabel, c);
	systemCard.add(sysYticLabel);
	
	c.gridx = 1;
	c.gridwidth = GridBagConstraints.REMAINDER;
	gridbag.setConstraints(systemYticDivision, c);
	systemCard.add(systemYticDivision);
	
	c.gridx = 0;
	c.gridy = 6;
	c.anchor = GridBagConstraints.CENTER;
	c.gridwidth = GridBagConstraints.REMAINDER;
	gridbag.setConstraints(systemUpdateButton, c);
	systemCard.add(systemUpdateButton);

	// add components to the output panel
	//
	c.gridx = 0;
	c.gridy = 0;
	c.gridwidth = 1;
	outputCard.setLayout(gridbag);
	gridbag.setConstraints(outXnegLabel, c);
	outputCard.add(outXnegLabel);
	
	c.gridx = 1;
	c.gridwidth = GridBagConstraints.REMAINDER;
	gridbag.setConstraints(outputXnegLimit, c);
	outputCard.add(outputXnegLimit);
	
	c.gridx = 0;
	c.gridy = 1;
	c.gridwidth = 1;
	gridbag.setConstraints(outXposLabel, c);
	outputCard.add(outXposLabel);
	
	c.gridx = 1;
	c.gridwidth = GridBagConstraints.REMAINDER;
	gridbag.setConstraints(outputXposLimit, c);
	outputCard.add(outputXposLimit);
	
	c.gridx = 0;
	c.gridy = 2;
	c.gridwidth = 1;
	gridbag.setConstraints(outXticLabel, c);
	outputCard.add(outXticLabel);
	
	c.gridx = 1;
	c.gridwidth = GridBagConstraints.REMAINDER;
	gridbag.setConstraints(outputXticDivision, c);
	outputCard.add(outputXticDivision);
	
	c.gridx = 0;
	c.gridy = 3;
	c.gridwidth = 1;
	gridbag.setConstraints(outYnegLabel, c);
	outputCard.add(outYnegLabel);
	
	c.gridx = 1;
	c.gridwidth = GridBagConstraints.REMAINDER;
	gridbag.setConstraints(outputYnegLimit, c);
	outputCard.add(outputYnegLimit);
	
	c.gridx = 0;
	c.gridy = 4;
	c.gridwidth = 1;
	gridbag.setConstraints(outYposLabel, c);
	outputCard.add(outYposLabel);
	
	c.gridx = 1;
	c.gridwidth = GridBagConstraints.REMAINDER;
	gridbag.setConstraints(outputYposLimit, c);
	outputCard.add(outputYposLimit);
	
	c.gridx = 0;
	c.gridy = 5;
	c.gridwidth = 1;
	gridbag.setConstraints(outYticLabel, c);
	outputCard.add(outYticLabel);
	
	c.gridx = 1;
	c.gridwidth = GridBagConstraints.REMAINDER;
	gridbag.setConstraints(outputYticDivision, c);
	outputCard.add(outputYticDivision);
	
	c.gridx = 0;
	c.gridy = 6;
	c.anchor = GridBagConstraints.CENTER;
	c.gridwidth = GridBagConstraints.REMAINDER;
	gridbag.setConstraints(outputUpdateButton, c);
	outputCard.add(outputUpdateButton);

	// add components to the magnitude panel
	//
	c.gridx = 0;
	c.gridy = 0;
	c.gridwidth = 1;
	magnitudeCard.setLayout(gridbag);
	gridbag.setConstraints(magfreqLabel, c);
	magnitudeCard.add(magfreqLabel);
	
	c.gridx = 1;
	c.gridwidth = GridBagConstraints.REMAINDER;
	gridbag.setConstraints(magnitudefreqLimit, c);
	magnitudeCard.add(magnitudefreqLimit);

	c.gridx = 0;
	c.gridy = 1;
	c.gridwidth = 1;
	magnitudeCard.setLayout(gridbag);
	gridbag.setConstraints(magXnegLabel, c);
	magnitudeCard.add(magXnegLabel);
	
	c.gridx = 1;
	c.gridwidth = GridBagConstraints.REMAINDER;
	gridbag.setConstraints(magnitudeXnegLimit, c);
	magnitudeCard.add(magnitudeXnegLimit);
	
	c.gridx = 0;
	c.gridy = 2;
	c.gridwidth = 1;
	gridbag.setConstraints(magXposLabel, c);
	magnitudeCard.add(magXposLabel);
	
	c.gridx = 1;
	c.gridwidth = GridBagConstraints.REMAINDER;
	gridbag.setConstraints(magnitudeXposLimit, c);
	magnitudeCard.add(magnitudeXposLimit);
	
	c.gridx = 0;
	c.gridy = 3;
	c.gridwidth = 1;
	gridbag.setConstraints(magXticLabel, c);
	magnitudeCard.add(magXticLabel);
	
	c.gridx = 1;
	c.gridwidth = GridBagConstraints.REMAINDER;
	gridbag.setConstraints(magnitudeXticDivision, c);
	magnitudeCard.add(magnitudeXticDivision);
	
	c.gridx = 0;
	c.gridy = 4;
	c.gridwidth = 1;
	gridbag.setConstraints(magYnegLabel, c);
	magnitudeCard.add(magYnegLabel);
	
	c.gridx = 1;
	c.gridwidth = GridBagConstraints.REMAINDER;
	gridbag.setConstraints(magnitudeYnegLimit, c);
	magnitudeCard.add(magnitudeYnegLimit);
	
	c.gridx = 0;
	c.gridy = 5;
	c.gridwidth = 1;
	gridbag.setConstraints(magYposLabel, c);
	magnitudeCard.add(magYposLabel);
	
	c.gridx = 1;
	c.gridwidth = GridBagConstraints.REMAINDER;
	gridbag.setConstraints(magnitudeYposLimit, c);
	magnitudeCard.add(magnitudeYposLimit);
	
	c.gridx = 0;
	c.gridy = 6;
	c.gridwidth = 1;
	gridbag.setConstraints(magYticLabel, c);
	magnitudeCard.add(magYticLabel);
	
	c.gridx = 1;
	c.gridwidth = GridBagConstraints.REMAINDER;
	gridbag.setConstraints(magnitudeYticDivision, c);
	magnitudeCard.add(magnitudeYticDivision);
	
	c.gridx = 0;
	c.gridy = 7;
	c.anchor = GridBagConstraints.CENTER;
	c.gridwidth = GridBagConstraints.REMAINDER;
	gridbag.setConstraints(magnitudeUpdateButton, c);
	magnitudeCard.add(magnitudeUpdateButton);

	// add title borders to each card in the layout
	//
	Border titleBorder = BorderFactory.createTitledBorder(
				 BorderFactory.createEmptyBorder(), 
				 signalTitle, 
				 TitledBorder.LEFT, 
				 TitledBorder.ABOVE_TOP, 
				 newFont, 
				 Color.black);
	signalCard.setBorder(titleBorder);
	
	titleBorder = BorderFactory.createTitledBorder(
				 BorderFactory.createEmptyBorder(), 
				 systemTitle, 
				 TitledBorder.LEFT, 
				 TitledBorder.ABOVE_TOP, 
				 newFont, 
				 Color.black);
	systemCard.setBorder(titleBorder);

	titleBorder = BorderFactory.createTitledBorder(
				 BorderFactory.createEmptyBorder(), 
				 outputTitle, 
				 TitledBorder.LEFT, 
				 TitledBorder.ABOVE_TOP, 
				 newFont, 
				 Color.black);
	outputCard.setBorder(titleBorder);

	titleBorder = BorderFactory.createTitledBorder(
				 BorderFactory.createEmptyBorder(), 
				 magnitudeTitle, 
				 TitledBorder.LEFT, 
				 TitledBorder.ABOVE_TOP, 
				 newFont, 
				 Color.black);
	magnitudeCard.setBorder(titleBorder);

	// add the control and parameters panel to the card
	//
	cards.add(signalCard, signalParams);
	cards.add(systemCard, systemParams);
	cards.add(outputCard, outputParams);
	cards.add(magnitudeCard, magnitudeParams);
	
	// add the cards and the main panel to content pane
	//
	constrain(this, cards, 0, 0, 
		  GridBagConstraints.REMAINDER, 1, 
		  GridBagConstraints.BOTH,
		  GridBagConstraints.NORTHEAST, 1.0, 1.0, 2, 2, 2, 2); 
    }
    
    /**
     * Empty implementation
     *
     * @param e events fired by the action listeners 
     */
    public void actionPerformed(ActionEvent e) {
	// this method is required to be present
    } 
    
    /**
     * check for state changes in the card layout
     *
     * @param evt event fired by the item listeners
     */
    public void itemStateChanged(ItemEvent evt) {      
	CardLayout cl = (CardLayout)(cards.getLayout());
	cl.show(cards, (String)evt.getItem());
    }
}

//
// end of file







