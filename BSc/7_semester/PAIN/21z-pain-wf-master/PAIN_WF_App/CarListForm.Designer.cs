
namespace PAIN_WF_App
{
    partial class CarListForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(CarListForm));
            this.toolStripContainer1 = new System.Windows.Forms.ToolStripContainer();
            this.carListView = new System.Windows.Forms.ListView();
            this.brand = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.maxSpeed = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.productionDate = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.Type = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.addContextMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editContextMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.deleteContextMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.carToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.deleteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStrip = new System.Windows.Forms.ToolStrip();
            this.addCarButton = new System.Windows.Forms.ToolStripButton();
            this.editCarButton = new System.Windows.Forms.ToolStripButton();
            this.deleteCarButton = new System.Windows.Forms.ToolStripButton();
            this.carFilterComboBox = new System.Windows.Forms.ToolStripComboBox();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.carCountStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripContainer1.BottomToolStripPanel.SuspendLayout();
            this.toolStripContainer1.ContentPanel.SuspendLayout();
            this.toolStripContainer1.TopToolStripPanel.SuspendLayout();
            this.toolStripContainer1.SuspendLayout();
            this.contextMenuStrip1.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.toolStrip.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // toolStripContainer1
            // 
            // 
            // toolStripContainer1.BottomToolStripPanel
            // 
            this.toolStripContainer1.BottomToolStripPanel.Controls.Add(this.statusStrip1);
            // 
            // toolStripContainer1.ContentPanel
            // 
            this.toolStripContainer1.ContentPanel.Controls.Add(this.carListView);
            this.toolStripContainer1.ContentPanel.Size = new System.Drawing.Size(568, 245);
            this.toolStripContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.toolStripContainer1.Location = new System.Drawing.Point(0, 0);
            this.toolStripContainer1.Name = "toolStripContainer1";
            this.toolStripContainer1.Size = new System.Drawing.Size(568, 267);
            this.toolStripContainer1.TabIndex = 0;
            this.toolStripContainer1.Text = "toolStripContainer1";
            // 
            // toolStripContainer1.TopToolStripPanel
            // 
            this.toolStripContainer1.TopToolStripPanel.Controls.Add(this.menuStrip1);
            this.toolStripContainer1.TopToolStripPanel.Controls.Add(this.toolStrip);
            // 
            // carListView
            // 
            this.carListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.brand,
            this.maxSpeed,
            this.productionDate,
            this.Type});
            this.carListView.ContextMenuStrip = this.contextMenuStrip1;
            this.carListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.carListView.FullRowSelect = true;
            this.carListView.HideSelection = false;
            this.carListView.Location = new System.Drawing.Point(0, 0);
            this.carListView.Name = "carListView";
            this.carListView.Size = new System.Drawing.Size(568, 245);
            this.carListView.TabIndex = 0;
            this.carListView.UseCompatibleStateImageBehavior = false;
            this.carListView.View = System.Windows.Forms.View.Details;
            this.carListView.SelectedIndexChanged += new System.EventHandler(this.carListView_SelectedIndexChanged);
            // 
            // brand
            // 
            this.brand.Text = "Brand";
            // 
            // maxSpeed
            // 
            this.maxSpeed.Text = "Max Speed";
            // 
            // productionDate
            // 
            this.productionDate.Text = "Production Date";
            // 
            // Type
            // 
            this.Type.Text = "Type";
            // 
            // contextMenuStrip1
            // 
            this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addContextMenuItem,
            this.editContextMenuItem,
            this.deleteContextMenuItem});
            this.contextMenuStrip1.Name = "contextMenuStrip1";
            this.contextMenuStrip1.Size = new System.Drawing.Size(108, 70);
            // 
            // addContextMenuItem
            // 
            this.addContextMenuItem.Name = "addContextMenuItem";
            this.addContextMenuItem.Size = new System.Drawing.Size(107, 22);
            this.addContextMenuItem.Text = "Add";
            this.addContextMenuItem.Click += new System.EventHandler(this.addContextMenuItem_Click);
            // 
            // editContextMenuItem
            // 
            this.editContextMenuItem.Name = "editContextMenuItem";
            this.editContextMenuItem.Size = new System.Drawing.Size(107, 22);
            this.editContextMenuItem.Text = "Edit";
            this.editContextMenuItem.Click += new System.EventHandler(this.editContextMenuItem_Click);
            // 
            // deleteContextMenuItem
            // 
            this.deleteContextMenuItem.Name = "deleteContextMenuItem";
            this.deleteContextMenuItem.Size = new System.Drawing.Size(107, 22);
            this.deleteContextMenuItem.Text = "Delete";
            this.deleteContextMenuItem.Click += new System.EventHandler(this.deleteContextMenuItem_Click);
            // 
            // menuStrip1
            // 
            this.menuStrip1.Dock = System.Windows.Forms.DockStyle.None;
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.carToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(45, 24);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            this.menuStrip1.Visible = false;
            // 
            // carToolStripMenuItem
            // 
            this.carToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addToolStripMenuItem,
            this.editToolStripMenuItem1,
            this.deleteToolStripMenuItem});
            this.carToolStripMenuItem.Name = "carToolStripMenuItem";
            this.carToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.carToolStripMenuItem.Text = "Car";
            // 
            // addToolStripMenuItem
            // 
            this.addToolStripMenuItem.Name = "addToolStripMenuItem";
            this.addToolStripMenuItem.Size = new System.Drawing.Size(107, 22);
            this.addToolStripMenuItem.Text = "Add";
            this.addToolStripMenuItem.Click += new System.EventHandler(this.addToolStripMenuItem_Click);
            // 
            // editToolStripMenuItem1
            // 
            this.editToolStripMenuItem1.Name = "editToolStripMenuItem1";
            this.editToolStripMenuItem1.Size = new System.Drawing.Size(107, 22);
            this.editToolStripMenuItem1.Text = "Edit";
            this.editToolStripMenuItem1.Click += new System.EventHandler(this.editToolStripMenuItem1_Click);
            // 
            // deleteToolStripMenuItem
            // 
            this.deleteToolStripMenuItem.Name = "deleteToolStripMenuItem";
            this.deleteToolStripMenuItem.Size = new System.Drawing.Size(107, 22);
            this.deleteToolStripMenuItem.Text = "Delete";
            this.deleteToolStripMenuItem.Click += new System.EventHandler(this.deleteToolStripMenuItem_Click);
            // 
            // toolStrip
            // 
            this.toolStrip.Dock = System.Windows.Forms.DockStyle.None;
            this.toolStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addCarButton,
            this.editCarButton,
            this.deleteCarButton,
            this.carFilterComboBox});
            this.toolStrip.Location = new System.Drawing.Point(3, 24);
            this.toolStrip.Name = "toolStrip";
            this.toolStrip.Size = new System.Drawing.Size(322, 38);
            this.toolStrip.TabIndex = 0;
            this.toolStrip.Visible = false;
            // 
            // addCarButton
            // 
            this.addCarButton.Image = ((System.Drawing.Image)(resources.GetObject("addCarButton.Image")));
            this.addCarButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.addCarButton.Name = "addCarButton";
            this.addCarButton.Size = new System.Drawing.Size(33, 35);
            this.addCarButton.Text = "Add";
            this.addCarButton.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.addCarButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.addCarButton.Click += new System.EventHandler(this.addCarButton_Click);
            // 
            // editCarButton
            // 
            this.editCarButton.Image = ((System.Drawing.Image)(resources.GetObject("editCarButton.Image")));
            this.editCarButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.editCarButton.Name = "editCarButton";
            this.editCarButton.Size = new System.Drawing.Size(31, 35);
            this.editCarButton.Text = "Edit";
            this.editCarButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.editCarButton.Click += new System.EventHandler(this.editCarButton_Click);
            // 
            // deleteCarButton
            // 
            this.deleteCarButton.Image = ((System.Drawing.Image)(resources.GetObject("deleteCarButton.Image")));
            this.deleteCarButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.deleteCarButton.Name = "deleteCarButton";
            this.deleteCarButton.Size = new System.Drawing.Size(44, 35);
            this.deleteCarButton.Text = "Delete";
            this.deleteCarButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageAboveText;
            this.deleteCarButton.Click += new System.EventHandler(this.deleteCarButton_Click);
            // 
            // carFilterComboBox
            // 
            this.carFilterComboBox.Items.AddRange(new object[] {
            "All",
            "Max speed less than 100km/h",
            "Max speed at least 100km/h"});
            this.carFilterComboBox.MaxLength = 3;
            this.carFilterComboBox.Name = "carFilterComboBox";
            this.carFilterComboBox.Size = new System.Drawing.Size(200, 38);
            this.carFilterComboBox.SelectedIndexChanged += new System.EventHandler(this.carFilterComboBox_SelectedIndexChanged);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Dock = System.Windows.Forms.DockStyle.None;
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.carCountStatusLabel});
            this.statusStrip1.Location = new System.Drawing.Point(0, 0);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(568, 22);
            this.statusStrip1.TabIndex = 0;
            // 
            // carCountStatusLabel
            // 
            this.carCountStatusLabel.Name = "carCountStatusLabel";
            this.carCountStatusLabel.Size = new System.Drawing.Size(13, 17);
            this.carCountStatusLabel.Text = "0";
            // 
            // CarListForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.AutoValidate = System.Windows.Forms.AutoValidate.EnableAllowFocusChange;
            this.ClientSize = new System.Drawing.Size(568, 267);
            this.Controls.Add(this.toolStripContainer1);
            this.MainMenuStrip = this.menuStrip1;
            this.MinimumSize = new System.Drawing.Size(50, 39);
            this.Name = "CarListForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "CarListForm";
            this.Activated += new System.EventHandler(this.CarListForm_Activated);
            this.Deactivate += new System.EventHandler(this.CarListForm_Deactivate);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.CarListForm_FormClosing);
            this.Load += new System.EventHandler(this.CarListForm_Load);
            this.toolStripContainer1.BottomToolStripPanel.ResumeLayout(false);
            this.toolStripContainer1.BottomToolStripPanel.PerformLayout();
            this.toolStripContainer1.ContentPanel.ResumeLayout(false);
            this.toolStripContainer1.TopToolStripPanel.ResumeLayout(false);
            this.toolStripContainer1.TopToolStripPanel.PerformLayout();
            this.toolStripContainer1.ResumeLayout(false);
            this.toolStripContainer1.PerformLayout();
            this.contextMenuStrip1.ResumeLayout(false);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.toolStrip.ResumeLayout(false);
            this.toolStrip.PerformLayout();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ToolStripContainer toolStripContainer1;
        private System.Windows.Forms.ListView carListView;
        private System.Windows.Forms.ColumnHeader brand;
        private System.Windows.Forms.ColumnHeader maxSpeed;
        private System.Windows.Forms.ColumnHeader productionDate;
        private System.Windows.Forms.ColumnHeader Type;
        private System.Windows.Forms.ToolStrip toolStrip;
        private System.Windows.Forms.ToolStripButton addCarButton;
        private System.Windows.Forms.ToolStripButton editCarButton;
        private System.Windows.Forms.ToolStripButton deleteCarButton;
        private System.Windows.Forms.ToolStripComboBox carFilterComboBox;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem carToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem addToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem deleteToolStripMenuItem;
        private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
        private System.Windows.Forms.ToolStripMenuItem addContextMenuItem;
        private System.Windows.Forms.ToolStripMenuItem editContextMenuItem;
        private System.Windows.Forms.ToolStripMenuItem deleteContextMenuItem;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel carCountStatusLabel;
    }
}